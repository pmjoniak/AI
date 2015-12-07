
/*
	This is the best wumpus agent in the world!
	Yay!
*/

/*====================== some mechanics for programming confort =====================*/

act(Action, Knowledge) :-
	not(currentPosition(_, _)),
	% Creating initial knowledge
	assert(currentPosition(1, 1)),
	assert(currentDirection(east)),
	assert(hasGold(false)),
	assert(wumpusKilled(false)),
	assert(shouldExit(false)),
	assert(hasArrow(true)),
	assert(queue([])),
	act(Action, Knowledge).

% we handle the knowledge-passing automatically and only concentrate on the actions
act(Action, Knowledge) :-
	preprocess,
	act(Action),
	forward_knowledge(Knowledge).

forward_knowledge(Knowledge) :-
	goal(X,Y),
	forward_knowledge_base(KnowledgeBase),
	Knowledge = [goal(X,Y) | KnowledgeBase].

forward_knowledge(Knowledge) :-
	forward_knowledge_base(Knowledge).

forward_knowledge_base(Knowledge) :-
	wumpusKilled(WumpusKilled),
	hasGold(HasGold),
	shouldExit(ShouldExit),
	worldSize(WorldX,WorldY),
	currentPosition(PosX, PosY),
	currentDirection(Direction),
	hasArrow(HasArrow),
	queue(Path),
	Knowledge = [
		wumpusKilled(WumpusKilled),
		hasGold(HasGold),
		shouldExit(ShouldExit),
		worldSize(WorldX,WorldY),
		currentPosition(PosX, PosY),
		currentDirection(Direction),
		queue(Path),
		hasArrow(HasArrow) | Tail
	],
	accumulate_board(Tail).

% forwarding of board knowledge
accumulate_board(Board) :-
	setof(board(X,Y,Type), board(X,Y,Type), Board).
/*
set_to(Predicate, Val) :-
	retract(Predicate(_)),
	assert(Predicate(Val)).
*/
/*===================== actual program =========================================*/

/*-------------- mechanics for remembering the percepts of the world -------------*/

move_forward(Action) :-
	not(is_facing_wall),
	Action = moveForward,
	currentPosition(PosX,PosY),
	(
		(
			currentDirection(east),
			PosXnew is PosX + 1,
			PosYnew = PosY
		) ;
		(
			currentDirection(north),
			PosXnew = PosX,
			PosYnew is PosY + 1
		) ;
		(
			currentDirection(west),
			PosXnew is PosX - 1,
			PosYnew = PosY
		) ;
		(
			currentDirection(south),
			PosXnew = PosX,
			PosYnew is PosY - 1
		)
	),
	retract(currentPosition(PosX,PosY)),
	assert(currentPosition(PosXnew, PosYnew)).

% we shoot the arrow (but only if we have it)
shoot_arrow(Action) :-
	hasArrow(true),
	Action = shoot,
	retract(hasArrow(true)),
	assert(hasArrow(false)).


/*------------------ preprocessing for remembering the state of the board ----------------*/

preprocess :-
	find_wumpus,
	preprocess.

% if wumpus is dead, we consider stinking fields as free (and go on preprocessing)
preprocess :-
	wumpusKilled(true),
	board(X,Y,stench),
	retract(board(X,Y,stench)),
	assert(board(X,Y,free)),
	preprocess.

% if we have information about the current field, preprocessing is done
preprocess :-
	currentPosition(Xpos, Ypos),
	board(Xpos, Ypos, _).

% we save information about stenches and breezes
preprocess :-
	wumpusKilled(false),
	stench,
	breeze,
	currentPosition(Xpos, Ypos),
	assert(board(Xpos, Ypos, stench)),
	assert(board(Xpos, Ypos, breeze)).	

preprocess :-
	wumpusKilled(false),
	stench,
	currentPosition(Xpos, Ypos),
	assert(board(Xpos, Ypos, stench)).

preprocess :-
	breeze,
	currentPosition(Xpos, Ypos),
	assert(board(Xpos, Ypos, breeze)).

% if there were no stenches nor breezes, we assume the field is free (this will alwayssucceed)
preprocess :-	
	currentPosition(Xpos, Ypos),
	assert(board(Xpos, Ypos, free)).

/*------------------------------ helper predicates ------------------------------*/

% gives an unexplored-but-reachable field
unexplored(X,Y) :-
	board(Nx,Ny,free),
	neighbour(X, Y, Nx, Ny),
	not(board(X,Y,_)).


% determines if a field is safe to move upon
% fields we have explored are safe
is_safe(X, Y) :-
	board(X,Y,free);
	board(X,Y,breeze);
	board(X,Y,stench).

% a field is considered safe if it has an empty neighbour
is_safe(X,Y) :-
	neighbour(Nx, Ny, X, Y),
	board(Nx,Ny,free).

% gives current field neighbours
neighbour_curent(X,Y) :-
	currentPosition(Xpos, Ypos),
	neighbour(X,Y,Xpos,Ypos).

% determines if a field is neighbouring a given field
neighbour(X,Y,Xpos,Ypos) :-
	(
		(
			X is Xpos - 1,
			Y = Ypos
		);
		(
			X is Xpos + 1,
			Y = Ypos
		);
		(
			X = Xpos,
			Y is Ypos + 1
		);
		(
			X = Xpos,
			Y is Ypos - 1
		)
	),
	is_on_board(X,Y).

explored(X,Y) :-
	board(X,Y,_).

direction(X, Y, Xcurr, Ycurr, Orientation, Direction) :-
	DeltaX is X - Xcurr,
	DeltaY is Y - Ycurr,
	(
		(
			DeltaX > 0,
			Target = east
		);
		(
			DeltaY > 0,
			Target = north
		);
		(
			DeltaX < 0,
			Target = west
		);
		(
			DeltaY < 0,
			Target = south
		)
	),
	orient_towards(Orientation, Direction, Target).

opposite_dir(west, east).
opposite_dir(east, west).
opposite_dir(north, south).
opposite_dir(south, north).

next_dir(north, east).
next_dir(east, south).
next_dir(south, west).
next_dir(west, north).

orient_towards(Orientation, Direction, Target) :-
	Target = Orientation,
	Direction = forward.

% if the target is opposite, we turn left or right
orient_towards(Orientation, Direction, Target) :-
	opposite_dir(Orientation, Target),
	(Direction = left;
	 Direction = right).

orient_towards(Orientation, right, Target) :-
	next_dir(Orientation, Target).

orient_towards(Orientation, left, Target) :-
	next_dir(Target, Orientation).

direction_current(X,Y,Direction) :-
	currentPosition(Xpos, Ypos),
	currentDirection(Orientation),
	direction(X,Y,Xpos,Ypos,Orientation,Direction).


% if the goal is a neighbour and it's in front of us, we walk towards it
act_upon_goal(Action) :-
	goal(Xgoal,Ygoal),
	direction_current(Xgoal, Ygoal, forward),
	retract(goal(_,_)),
	move_forward(Action).

act_upon_goal(Action) :-
	goal(Xgoal,Ygoal),
	direction_current(Xgoal, Ygoal, left),
	turn_left(Action).
	%retract(goal(_,_)).

act_upon_goal(Action) :-
	goal(Xgoal,Ygoal),
	direction_current(Xgoal, Ygoal, right),
	turn_right(Action).
	%retract(goal(_,_)).



turn_left(Action) :-
	Action = turnLeft,
	retract(currentDirection(Direction)),
	next_dir(NewDirection, Direction),
	assert(currentDirection(NewDirection)).

turn_right(Action) :-
	Action = turnRight,
	retract(currentDirection(Direction)),
	next_dir(Direction, NewDirection),
	assert(currentDirection(NewDirection)).


/*======================== priority actions ========================*/

act(Action) :- grab_gold(Action).

act(Action) :- currentPosition(1,1),
(	
	exit_if_on_beginning(Action);
	runaway_on_beginning_breeze(Action);
	shoot_wumpus_if_on_beginning(Action);
	assume_clear_front_field(Action)
).
act(Action) :- listen_for_wumpus_death(Action).
act(Action) :- act_upon_goal(Action).
act(Action) :- consume_queue_head(Action).
act(Action) :- find_exit(Action).
act(Action) :- kill_wumpus(Action).
act(Action) :- explore(Action).
%act(Action) :- explore_unexplored_neighbours(Action).
%act(Action) :- explore_unexplored_field(Action).
act(Action) :- exit_if_theres_nothing_to_do(Action).

% if there's a breeze at the beginning, it's not worth it
runaway_on_beginning_breeze(Action) :-
	breeze,
	currentPosition(1,1),
	Action = exit.

% if we should exit and are on the beginning, we exit
exit_if_on_beginning(Action) :-
	shouldExit(true),
	currentPosition(1,1),
	Action = exit.

% if we're on the gold, we grab it and express the wish to get out
grab_gold(Action) :- 
	glitter,
	Action = grab,
	retract(hasGold(_)),
	assert(hasGold(true)),
	retract(shouldExit(_)),
	assert(shouldExit(true)).

% if we have no goal, but have something in the queue, we take it from the queue and set is as the goal
consume_queue_head(Action) :-
	not(goal(_,_)),
	queue([_|T]),
	retract(queue([(Hx,Hy)|T])),
	assert(goal(Hx,Hy)),
	assert(queue(T)),
	act(Action).


% if we wish to get out, but don't know how, we find the path to the start
find_exit(Action) :-
	shouldExit(true),
	not(goal(_,_)),
	queue([]),
	currentPosition(X,Y),
	shortest_path(X,Y,1,1,Path),
	retract(queue([])),
	assert(queue(Path)),
	act(Action).


% if we hear a scream, we note that wumpus has been killed and continue to act upon that fact
listen_for_wumpus_death(Action) :-
	scream,
	wumpusKilled(false),
	retract(wumpusKilled(_)),
	assert(wumpusKilled(true)),
	act(Action).

% if we smell wumpus when standing on the beginning, we try a shot
shoot_wumpus_if_on_beginning(Action) :-
	stench,
	currentPosition(1,1),
	wumpusKilled(false),
	shoot_arrow(Action).

% if the wumpus is alive and we cannot shoot, the field in the front is probably clear!
assume_clear_front_field(Action) :-
	stench,
	not(breeze),
	currentPosition(1,1),
	wumpusKilled(false),
	assert(goal(2,1)),
	act(Action).


/*
act(Action) :-
	neighbour_curent(X,Y),
	unexplored(X,Y),
	is_safe(X,Y),
	assert(goal(X,Y)),
	act(Action).
*/

explore_unexplored_neighbours(Action) :-
	shouldExit(false),
	neighbour_curent(X,Y),
	unexplored(X,Y),
	not(goal(_,_)),
	queue([]),
	assert(goal(X,Y)),
	act(Action).



explore_unexplored_field(Action) :-
	shouldExit(false),
	not(goal(_,_)),
	queue([]),
	unexplored(X,Y),
	currentPosition(Cx,Cy),
	shortest_path(Cx,Cy,X,Y,Path),
	retract(queue([])),
	assert(queue(Path)),
	act(Action).


exit_if_theres_nothing_to_do(Action) :-
	shouldExit(false),
	retract(shouldExit(_)),
	assert(shouldExit(true)),
	%Action = turnLeft.
	act(Action).

/*
% if we have an unexplored, safe neighbour, we are eager to explore it
act(Action) :-
	not(goal(_,_)),
	neighbour_curent(X,Y),
	not(board(X,Y,_)),
	is_safe(X,Y),
	assert(goal(X,Y)),
	act(Action).
	*/
/*
% if we got into trouble, we go to any safe neighbours we might have
act(Action) :-
	not(goal(_,_)),
	neighbour_curent(X,Y),
	is_safe(X,Y),
	assert(goal(X,Y)),
	act(Action).
*/


is_on_board(X,Y) :-
	worldSize(SizeX, SizeY),
	X =< SizeX,
	Y =< SizeY,
	X >= 1,
	Y >= 1.

% this is used for determining whether the agent is facing the wall
is_facing_wall :-
	worldSize(SizeX, SizeY),
	currentPosition(PosX, PosY),
	(
		(currentDirection(east),
		SizeX = PosX)
		;
		(currentDirection(west),
		PosX = 1)
		;
		(currentDirection(north),
		PosY = SizeY)
		;
		(currentDirection(south),
		PosY = 1)
	).

in_trouble :-
	breeze.

in_trouble :-
	stench,
	wumpusKilled(false).





% unifies Dist with a manhattan distance from(X,Y) to (X2,Y2)
distance(X,Y,X2,Y2,Dist) :-
	Dx is abs(X-X2),
	Dy is abs(Y-Y2),
	Dist is Dx+Dy.

/*
astar(Xstart, Ystart, Xend, Yend, Path) :-
	


find_cheapest(L, X, Y, Val) :-
	L = [(Hx, Hy, Hval)|T],
	find_cheapest(T, Tx, Ty, Tval),
	(Hval > Tval ->()
		Val = Hval,
		X = Hx,
		Y = Hy);
		Val = Tval,
		X = Tx,
		Y = Ty).

expand_node(StartX, StartY, EndX, EndY) :-
	neighbour(X, Y, NodeX, NodeY),
	is_safe(X,Y),
	distance(X,Y,StartX,StartY,StartDist),
	distance(X,Y,EndX,EndY,EndDist),
	TotalDist is StartDist + EndDist,
	assert()
*/

explore(Action) :- 
	not(goal(_,_)),
	queue([]),
	closest_unexplored_neighbour(X,Y),
	assert(goal(X,Y)),
	act(Action).

explore(Action) :- 
	not(goal(_,_)),
	queue([]),
	closest_field(X,Y),
	currentPosition(Cx,Cy),
	shortest_path(Cx,Cy,X,Y,Path),
	retract(queue([])),
	assert(queue(Path)),
	act(Action).


closest_unexplored_neighbour(X,Y) :-
	neighbour_curent(X,Y),
	unexplored(X,Y),
	direction_current(X, Y, forward).

closest_unexplored_neighbour(X,Y) :-
	neighbour_curent(X,Y),
	unexplored(X,Y),
	direction_current(X, Y, left).

closest_unexplored_neighbour(X,Y) :-
	neighbour_curent(X,Y),
	unexplored(X,Y),
	direction_current(X, Y, right).

closest_unexplored_neighbour(X,Y) :-
	neighbour_curent(X,Y),
	unexplored(X,Y).

closest_field(X,Y) :-
	setof((X,Y), unexplored(X,Y), Unexplored),
	closest((X,Y), Unexplored).

closest(H, [H]).
closest(Res, [(Hx, Hy)|T]) :-
	currentPosition(Cx, Cy),
	closest((Tx, Ty), T),
	distance(Cx,Cy,Hx,Hy,Hdist),
	distance(Cx,Cy,Tx,Ty,Tdist),
	(Hdist < Tdist ->
		Res = (Hx,Hy);
		Res = (Tx,Ty)).

% wumpus is neighbour!
kill_wumpus(Action) :-
	currentPosition(Cx, Cy),
	board(X,Y,wumpus),
	neighbour(X,Y,Cx,Cy),	
	(
		(
			direction_current(X, Y, forward),
			Action = shoot,
			retract(board(X,Y,wumpus))
		);
		(
			direction_current(X, Y, left),
			turn_left(Action)
		);
		(
			direction_current(X, Y, right),
			turn_right(Action)
		)
	).


kill_wumpus(Action) :-
	not(goal(_,_)),
	currentPosition(Cx, Cy),
	board(X,Y,wumpus),
	neighbour(Nx,Ny,X,Y),
	setof((Nx,Ny), is_safe(Nx,Ny), Safes),
	closest((Gx,Gy), Safes),
	set_target(Gx,Gy),
	act(Action).


find_wumpus :-
	not(board(_,_,wumpus)),
	setof((X,Y), board(X,Y,stench), Stinking),
	member((X1,Y1), Stinking),
	member((X2,Y2), Stinking),
	(
		(
			X1 is X2 - 2,
			Wx is X1 + 1,
			Wy = Y2
		);
		(
			X2 is X1 - 2,
			Wx is X2 + 1,
			Wy = Y2
		);
		(
			Y1 is Y2 - 2,
			Wy is Y1 + 1,
			Wx = X1
		);
		(	
			Y2 is Y1 - 2,
			Wy is Y2 + 1,
			Wx = X1
		)
	),
	assert(board(Wx,Wy,wumpus)).

set_target(Gx,Gy) :-
	not(goal(_,_)),
	queue([]),
	currentPosition(Cx, Cy),
	neighbour_curent(Gx,Gy) ->
	(
		assert(goal(Gx,Gy)) 
	);
	(
		shortest_path(Cx,Cy,Gx,Gy,Path),
		retract(queue([])),
		assert(queue(Path))
	).

/*------------------------------ A* algorithm -------------------------*/


shortest_path(X,Y,Xend,Yend,[(Xend, Yend)]) :-
	neighbour(X,Y,Xend,Yend).


shortest_path(X,Y,Xend,Yend,Path) :-
	H is abs(X-Xend) + abs(Y-Yend),
	path(Xend, Yend, [], [(X,Y,0,H)],Path).

path(Xend, Yend, Explored, _, Path) :-
	member((Xend,Yend,_,_), Explored),
	find_path(Explored, Xend,Yend,Path).
	%reverse(PathRev, Path).

path(Xend, Yend, Explored, Frontier, Path) :-
	cheapest(Node, Frontier),
	Explored2 = [Node|Explored],
	delete(Node, Frontier,Frontier2),
	append(Explored, Frontier, Excluded),
	(
	neighbours(Node, Excluded, Xend, Yend, Neighbours);
	Neighbours = []
	),
	append(Neighbours, Frontier2, Frontier3),
	path(Xend, Yend, Explored2, Frontier3, Path).

cheapest(Node, [Node]).
cheapest(Node, [Head|T]) :-
	Head = (_,_,G,H),
	cheapest(Node2, T),
	Node2 = (_,_,G2,H2),
	Val1 is G+H,
	Val2 is G2+H2,
	(Val1 < Val2 ->
		Node = Head
		;
		Node = Node2
	).

safe_neighbour(Node, Excluded, Xend, Yend, Neighbour) :-
	Node = (Xn, Yn, G, _),
	neighbour(Nx,Ny,Xn,Yn),
	is_safe(Nx,Ny),
	not(member((Nx,Ny,_,_), Excluded)),
	Ng is G+1,
	Nh is abs(Xend-Nx) + abs(Yend-Ny),
	Neighbour = (Nx,Ny,Ng,Nh).

neighbours(Node, Excluded, Xend, Yend, Neighbours) :-
	setof(Neighbour, safe_neighbour(Node, Excluded, Xend, Yend, Neighbour), Neighbours).

explored_neighbour(Nx,Ny,G,X,Y,Explored) :-
	neighbour(Nx,Ny,X,Y),
	member((Nx,Ny,G,_), Explored).

cheapest_neighbour(Explored, Xend, Yend, Nx, Ny) :-
	setof((X,Y,G,0), explored_neighbour(X,Y,G,Xend,Yend,Explored), Neighbours),
	cheapest((Nx,Ny,_,_), Neighbours).

find_path(Explored, X, Y, []) :-
	member((X,Y,0,_), Explored).

find_path(Explored, Xend, Yend, Path) :-
	cheapest_neighbour(Explored, Xend,Yend,Nx,Ny),
	find_path(Explored, Nx, Ny, Path2),
	append(Path2, [(Xend,Yend)], Path).
