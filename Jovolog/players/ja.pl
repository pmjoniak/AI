% auxiliary initial action generating rule
act(Action, Knowledge) :-

	% To avoid looping on act/2.
	not(gameStarted),
	assert(gameStarted),

	% Creating initial knowledge
	worldSize(X,Y),				%this is given
	assert(myWorldSize(X,Y)),
	assert(myPosition(1, 1, east)),		%this we assume by default
	assert(haveGold(0)),
	assert(nothing_to_do_here(false)),
	assert(currentPath([])),
	assert(goal(1,1)),
	assert(wumpus_state(alive)),
	assert(wumpus_position([])),
	fillboard(X,Y),
	fail.

fillboard(X,Y) :-(fillrow(X, Y); true), NewX is X-1, NewX>0, fillboard(NewX,Y).

fillrow(X, Y) :- assert(board(X, Y, not_visited)), NewY is Y-1, NewY>0,fillrow(X, NewY).

accumulate_board(Board) :- setof(board(X,Y,Type), board(X,Y,Type), Board).

sum_knowledge(Knowledge) :-
	haveGold(NGolds),
	myWorldSize(Max_X,Max_Y),
	myPosition(X, Y, Orient),
	nothing_to_do_here(NTDH),
	currentPath(Path),
	goal(GX,GY),
	wumpus_state(WS),
	wumpus_position(WP),
	Knowledge = [gameStarted,
		     haveGold(NGolds),
	         myWorldSize(Max_X, Max_Y),
		     myPosition(X, Y, Orient),
			 nothing_to_do_here(NTDH),
			 goal(GX,GY),
			 wumpus_state(WS),
			 wumpus_position(WP),
			 currentPath(Path) | Board], accumulate_board(Board).


no_wumpus(X,Y) :- wumpus_state(dead);(myWorldSize(MaxX, MaxY), ((no_wumpusD1(MaxX, MaxY, X, Y); 
					no_wumpusD2(MaxX, MaxY, X, Y); no_wumpusD3(MaxX, MaxY, X, Y); no_wumpusD4(MaxX, MaxY, X, Y)); board(X,Y,free))).

no_wumpusD1(MaxX, MaxY, X,Y) :- NewX is X-1, NewX > 0, NewX =< MaxX, board(NewX,Y,visited), not(board(NewX, Y, stench)).
no_wumpusD2(MaxX, MaxY, X,Y) :- NewX is X+1, NewX > 0, NewX =< MaxX, board(NewX,Y,visited), not(board(NewX, Y, stench)).
no_wumpusD3(MaxX, MaxY, X,Y) :- NewY is Y-1, NewY > 0, NewY =< MaxY, board(X,NewY,visited), not(board(X, NewY, stench)).
no_wumpusD4(MaxX, MaxY, X,Y) :- NewY is Y+1, NewY > 0, NewY =< MaxY, board(X,NewY,visited), not(board(X, NewY, stench)).

no_pit(X,Y) :- myWorldSize(MaxX, MaxY), ((no_pitD1(MaxX, MaxY, X, Y); 
					no_pitD2(MaxX, MaxY, X, Y); no_pitD3(MaxX, MaxY, X, Y); no_pitD4(MaxX, MaxY, X, Y)); board(X,Y,free)).

no_pitD1(MaxX, MaxY, X,Y) :- NewX is X-1, NewX > 0, NewX =< MaxX, board(NewX,Y,visited), not(board(NewX, Y, breeze)).
no_pitD2(MaxX, MaxY, X,Y) :- NewX is X+1, NewX > 0, NewX =< MaxX, board(NewX,Y,visited), not(board(NewX, Y, breeze)).
no_pitD3(MaxX, MaxY, X,Y) :- NewY is Y-1, NewY > 0, NewY =< MaxY, board(X,NewY,visited), not(board(X, NewY, breeze)).
no_pitD4(MaxX, MaxY, X,Y) :- NewY is Y+1, NewY > 0, NewY =< MaxY, board(X,NewY,visited), not(board(X, NewY, breeze)).	

update_state(Action, Knowledge) :- myPosition(X,Y,Orient), assert(board(X,Y,free)), assert(board(X,Y,visited)), retract(board(X,Y, not_visited)), 
									(
										(stench, assert(board(X,Y,stench)), fail);
										(breeze, assert(board(X,Y,breeze)), fail)
									), fail.

update_wumpus_dead(Action, Knowledge) :- scream, wumpus_state(alive), retract(wumpus_state(alive)), assert(wumpus_state(dead)), fail.							

in_board(X,Y) :- myWorldSize(MaxX, MaxY), X > 0, Y > 0, X =< MaxX, Y =< MaxY.


clear_safenv(Knowledge) :- board(X,Y,safe_nv), retract(board(X,Y,safe_nv)), fail.
setup_safenv(Knowledge) :- find_safe_not_visited_position(X,Y), assert(board(X,Y,safe_nv)), fail.
clear_safe(Knowledge) :- board(X,Y,safe), retract(board(X,Y,safe)), fail.
setup_safe(Knowledge) :- board(X,Y, visited), safe_pos(X,Y), assert(board(X,Y,safe)), fail.

safe_pos(X,Y) :- in_board(X,Y), no_pit(X,Y), no_wumpus(X,Y).

% standard action generating rules
% this is our agent's algorithm, the rules will be tried in order
%act(Action, Knowledge) :- cycki(dupa), Action = turnLeft, Knowledge = [].
act(Action, Knowledge) :- update_state(Action, Knowledge).
act(Action, Knowledge) :- update_wumpus_dead(Action, Knowledge).
%act(Action, Knowledge) :- clear_safe(Knowledge).
%act(Action, Knowledge) :- setup_safe(Knowledge).
%act(Action, Knowledge) :- clear_safenv(Knowledge).
%act(Action, Knowledge) :- setup_safenv(Knowledge).
act(Action, Knowledge) :- exit_if_home(Action, Knowledge). %if at home with gold
act(Action, Knowledge) :- pick_up_gold(Action, Knowledge). %if just found gold
act(Action, Knowledge) :- setup_possible_wumpus_position(Action).
act(Action, Knowledge) :- bagof(X, pw(X), PW), wumpus_position(WP), retract(wumpus_position(WP)), assert(wumpus_position(PW)), fail.
act(Action, Knowledge) :- kill_wumpus(Action, Knowledge).
act(Action, Knowledge) :- ((achieved_goal(1, 1), find_path(1, 1));true), pursue_goal(Action, Knowledge).
%act(Action, Knowledge) :- ((achieved_goal(1, 1), find_path(1, 1));true), Action=turnLeft, sum_knowledge(Knowledge).
act(Action, Knowledge) :- Action = exit, Knowledge = [].

find_path(Action, Knowledge) :-	(find_safe_not_visited_position(NewGoalX, NewGoalY), find_path_to_goal(NewGoalX, NewGoalY));
								(find_path_to_goal(1,1), retract(nothing_to_do_here(false)), assert(nothing_to_do_here(true))).
									 
find_path_to_goal(GoalX, GoalY) :- myPosition(X,Y, Orient), currentPath(FP), retract(currentPath(FP)),
								(
									(GoalX > X,(
									(is_right_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									(   is_up_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									( is_down_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									( is_left_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)))
									));
									(GoalX < X,(
									( is_left_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									(   is_up_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									( is_down_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									(is_right_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)))
									));
									(GoalY > Y,(
									(   is_up_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									(is_right_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									( is_left_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									( is_down_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)))
									));
									(GoalY < Y,(
									( is_down_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									(is_right_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									( is_left_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)));
									(   is_up_good(X,Y,GoalX,GoalY, [], FoundedPath), rev(FoundedPath,NFP), assert(currentPath(NFP)))
									))

								).

achieved_goal(Action, Knowledge) :- currentPath(FP), FP = [].

find_safe_not_visited_position(X, Y) :- board(X, Y, not_visited), safe_pos(X, Y).

pursue_goal(Action, Knowledge) :- currentPath(FP), 
									FP = [Head| Tail], myPosition(X,Y, Orient),
									Head = [GoalX, GoalY], 
									goal(GX,GY),
									retract(goal(GX,GY)),
									assert(goal(GoalX, GoalY)),
									(
									(GoalX < X, pursue_left(Action, Orient, X,Y, NewX, NewY, NewOrient));
									(GoalX > X, pursue_right(Action, Orient, X,Y, NewX, NewY, NewOrient));
									(GoalY < Y, pursue_down(Action, Orient, X,Y, NewX, NewY, NewOrient));
									(GoalY > Y, pursue_up(Action, Orient, X,Y, NewX, NewY, NewOrient))
									),
									((Action = moveForward, retract(currentPath(FP)), assert(currentPath(Tail))); true),
									retract(myPosition(X,Y,Orient)), assert(myPosition(NewX, NewY, NewOrient)), 
											sum_knowledge(Knowledge).

pursue_left(Action, west, X, Y, NewX, NewY, NewOrient) :- Action = moveForward, NewX is X - 1, NewY = Y, NewOrient = west.
pursue_left(Action, east, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = north.
pursue_left(Action, north, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = west.
pursue_left(Action, south, X, Y, NewX, NewY, NewOrient) :- Action = turnRight, NewX = X, NewY = Y, NewOrient = west.

pursue_right(Action, east, X, Y, NewX, NewY, NewOrient) :- Action = moveForward, NewX is X + 1, NewY = Y, NewOrient = east.
pursue_right(Action, west, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = south.
pursue_right(Action, north, X, Y, NewX, NewY, NewOrient) :- Action = turnRight, NewX = X, NewY = Y, NewOrient = east.
pursue_right(Action, south, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = east.

pursue_down(Action, south, X, Y, NewX, NewY, NewOrient) :- Action = moveForward, NewX = X, NewY is Y - 1, NewOrient = south.
pursue_down(Action, north, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = west.
pursue_down(Action, east, X, Y, NewX, NewY, NewOrient) :- Action = turnRight, NewX = X, NewY = Y, NewOrient = south.
pursue_down(Action, west, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = south.

pursue_up(Action, north, X, Y, NewX, NewY, NewOrient) :- Action = moveForward, NewX = X, NewY is Y + 1, NewOrient = north.
pursue_up(Action, south, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = east.
pursue_up(Action, east, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = north.
pursue_up(Action, west, X, Y, NewX, NewY, NewOrient) :- Action = turnRight, NewX = X, NewY = Y, NewOrient = north.

shiftOrient(east, north).
shiftOrient(north, west).
shiftOrient(west, south).
shiftOrient(south, east).

exit_if_home(Action, Knowledge) :-
	myPosition(1, 1, Orient),
	((haveGold(NGolds), NGolds > 0); (nothing_to_do_here(true))),
	Action = exit,				%done game
	Knowledge = [].				%irrelevant but required

pick_up_gold(Action, Knowledge) :-
	glitter,
	Action = grab,			    %this is easy, we are sitting on it
	haveGold(NGolds),		    %we must know how many golds we have
	retract(haveGold(NGolds)),
	NewNGolds is NGolds + 1,
	assert(haveGold(NewNGolds)),
	find_path_to_goal(1,1),
	retract(nothing_to_do_here(false)),
	assert(nothing_to_do_here(true)),
	sum_knowledge(Knowledge).

kill_wumpus(Action, Knowledge) :- wumpus_state(alive), is_wumpus_to_kill(WumpusPos),
									myPosition(X,Y, Orient), WumpusPos = [WX,WY], wumpus_position(WP), retract(wumpus_position(WP)), assert(wumpus_position([WX,WY])),
									board(X,Y,stench),
									((facing(X,Y,Orient,WX,WY), Action = shoot, sum_knowledge(Knowledge));
										(
											Action = turnLeft, myPosition(X,Y,Orient), shiftOrient(Orient, NewOrient),
											retract(myPosition(X,Y,Orient)), assert(myPosition(X,Y,NewOrient)),
											 sum_knowledge(Knowledge)
										)).


shift_orient_toward(X,Y,Orient, TX,TY, NewOrient, Action) :- (TX > X, ((Orient = north, Action = turnLeft);(Orient = south, Action = turnRight)), NewOrient = east);
															 (TX < X, ((Orient = north, Action = turnRight);(Orient = south, Action = turnLeft)), NewOrient = west);
															 (TY > Y, ((Orient = east, Action = turnLeft);(Orient = west, Action = turnRight)), NewOrient = north);
															 (TY < Y, ((Orient = east, Action = turnRight);(Orient = west, Action = turnLeft)), NewOrient = south).

facing(X,Y,Orient,TX,TY) :- (TX > X, Orient = east);
							(TX < X, Orient = west);
							(TY > Y, Orient = north);
							(TY < Y, Orient = south).

setup_possible_wumpus_position(Action) :- board(X,Y, stench), possible_wumups(X,Y,PW), assert(pw(PW)), fail.


sum_wumpus_possible(Wspolna) :- bagof(T, pw(T), PW), intersection_w(PW, Wspolna), !.

is_wumpus_to_kill(WumpusPos) :- sum_wumpus_possible(Wspolna),
								not(Wspolna = []), Wspolna = [H|T], T=[], WumpusPos = H, !.

possible_wumups(X,Y,PW) :- myWorldSize(MaxX, MaxY),
							((NewX is X - 1, X > 0,      board(NewX, Y, not_visited),  not(safe_pos(NewX,Y)),  PW1 = [[NewX,Y]]);PW1=[]),
							((NewX2 is X + 1, X =< MaxX, board(NewX2, Y, not_visited), not(safe_pos(NewX2,Y)), PW2 = [[NewX2,Y]|PW1]);PW2=PW1),
							((NewY is Y - 1, Y > 0,      board(X, NewY, not_visited),  not(safe_pos(X,NewY)),  PW3 = [[X,NewY]|PW2]);PW3=PW2),
							((NewY2 is Y + 1, Y =< MaxY, board(X, NewY2, not_visited), not(safe_pos(X,NewY2)), PW4 = [[X,NewY2]|PW3]);PW4=PW3),
							PW = PW4, !.

inter([], _, []).
inter([H1|T1], L2, [H1|Res]) :- (member(H1, L2), inter(T1, L2, Res)).
inter([_|T1], L2, Res) :- inter(T1, L2, Res).

intersection_w([], []).
intersection_w([H|T], Res) :- (intersection_w(H,T,Res), !).
intersection_w(H1, [], H1).
intersection_w(H1, [H2|Tail],Res) :- (inter(H1, H2, Tmp), intersection_w(Tmp, Tail, Res)).


can_reach(FromX, FromY, ToX, ToY, Tabu, FoundedPath) :- (FromX = ToX, FromY = ToY, FoundedPath = Tabu);
													
												(
													(ToX > FromX, 
													(
													is_right_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_down_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_up_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_left_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath)
													));
													(ToX < FromX, 
													(
													is_left_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_down_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_up_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_right_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath)
													));
													(ToY < FromY, 
													(
													is_down_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_left_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_right_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_up_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath)
													));		
													(ToY > FromY, 
													(
													is_up_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_left_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_right_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
													is_down_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath)
													))																										
												).

is_left_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath) :- (FromX = ToX, FromY = ToY, FoundedPath = Tabu);
												(NewX is FromX - 1, NewX > 0, safe_pos(NewX, FromY), not(is_on_list([NewX, FromY],Tabu)),
													NewTabu = [[NewX, FromY] | Tabu], can_reach(NewX, FromY, ToX, ToY, NewTabu, FoundedPath)).

is_right_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath) :- (FromX = ToX, FromY = ToY, FoundedPath = Tabu);
												(NewX is FromX + 1, safe_pos(NewX, FromY), not(is_on_list([NewX, FromY],Tabu)),
													NewTabu = [[NewX, FromY] | Tabu], can_reach(NewX, FromY, ToX, ToY, NewTabu, FoundedPath)).

is_down_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath) :- (FromX = ToX, FromY = ToY, FoundedPath = Tabu);
												(NewY is FromY - 1, NewY > 0, safe_pos(FromX, NewY), not(is_on_list([FromX, NewY],Tabu)),
													NewTabu = [[FromX, NewY] | Tabu], can_reach(FromX, NewY, ToX, ToY, NewTabu, FoundedPath)).

is_up_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath) :- (FromX = ToX, FromY = ToY, FoundedPath = Tabu);
												(NewY is FromY + 1, safe_pos(FromX, NewY), not(is_on_list([FromX, NewY],Tabu)),
													NewTabu = [[FromX, NewY] | Tabu], can_reach(FromX, NewY, ToX, ToY, NewTabu, FoundedPath)).


is_on_list(X,[X|Tail]).
is_on_list(X,[Y|Tail]):- is_on_list(X,Tail).

accRev([H|T],A,R):-  accRev(T,[H|A],R). 
accRev([],A,A).
rev(L,R):-  accRev(L,[],R).