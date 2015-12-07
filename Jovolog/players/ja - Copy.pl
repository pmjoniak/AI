/* -*- mode: Prolog; comment-column: 48 -*- */

/****************************************************************************
 *
 * Copyright (c) 2013 Witold Paluszynski
 *
 * I grant everyone the right to copy this program in whole or part, and
 * to use it for any purpose, provided the source is properly acknowledged.
 *
 * Udzielam kazdemu prawa do kopiowania tego programu w calosci lub czesci,
 * i wykorzystania go w dowolnym celu, pod warunkiem zacytowania zrodla.
 *
 ****************************************************************************/


/*
  This program implements a simple agent strategy for the wumpus world.
  The agent ignores all dangers of the wumpus world.
  The strategy is to go forward along the perimeter,
  turn left when reached the opposing wall,
  but first of all pick up gold if stumbled upon it,
  and exit the game if at home with gold.
  This version registers all steps on a stack, and uses it to reverse
  the actions after having found gold, thus properly returning home.

  Also demonstrates how to keep track of her own position and orientation.
  The agent assumes that the starting point is (1,1) and orientation "east".
*/

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
	assert(currentTabu([])),
	assert(currentGoal([1,1])),
	assert(nothing_to_do_here(false)),
	assert(going(left)),
	fillboard(X,Y),
	fail.

fillboard(X,Y) :-(fillrow(X, Y); true), NewX is X-1, NewX>0, fillboard(NewX,Y).

fillrow(X, Y) :- assert(board(X, Y, not_visited)), NewY is Y-1, NewY>0,fillrow(X, NewY).

accumulate_board(Board) :- setof(board(X,Y,Type), board(X,Y,Type), Board).

sum_knowledge(Knowledge) :-
	haveGold(NGolds),
	myWorldSize(Max_X,Max_Y),
	myPosition(X, Y, Orient),
	currentGoal(Goal),
	currentTabu(Tabu),
	nothing_to_do_here(NTDH),
	going(Dir),
	Knowledge = [gameStarted,
		     haveGold(NGolds),
	         myWorldSize(Max_X, Max_Y),
		     myPosition(X, Y, Orient),
		     currentTabu(Tabu),
			 nothing_to_do_here(NTDH),
			 going(Dir),
		     currentGoal(Goal) | Board], accumulate_board(Board).


no_wumpus(X,Y) :- myWorldSize(MaxX, MaxY), (no_wumpusD1(MaxX, MaxY, X, Y); 
					no_wumpusD2(MaxX, MaxY, X, Y); no_wumpusD3(MaxX, MaxY, X, Y); no_wumpusD4(MaxX, MaxY, X, Y)).

no_wumpusD1(MaxX, MaxY, X,Y) :- NewX is X-1, NewX > 0, NewX =< MaxX, board(NewX,Y,visited), not(board(NewX, Y, stench)).
no_wumpusD2(MaxX, MaxY, X,Y) :- NewX is X+1, NewX > 0, NewX =< MaxX, board(NewX,Y,visited), not(board(NewX, Y, stench)).
no_wumpusD3(MaxX, MaxY, X,Y) :- NewY is Y-1, NewY > 0, NewY =< MaxY, board(X,NewY,visited), not(board(X, NewY, stench)).
no_wumpusD4(MaxX, MaxY, X,Y) :- NewY is Y+1, NewY > 0, NewY =< MaxY, board(X,NewY,visited), not(board(X, NewY, stench)).

no_pit(X,Y) :- myWorldSize(MaxX, MaxY), (no_pitD1(MaxX, MaxY, X, Y); 
					no_pitD2(MaxX, MaxY, X, Y); no_pitD3(MaxX, MaxY, X, Y); no_pitD4(MaxX, MaxY, X, Y)).

no_pitD1(MaxX, MaxY, X,Y) :- NewX is X-1, NewX > 0, NewX =< MaxX, board(NewX,Y,visited), not(board(NewX, Y, breeze)).
no_pitD2(MaxX, MaxY, X,Y) :- NewX is X+1, NewX > 0, NewX =< MaxX, board(NewX,Y,visited), not(board(NewX, Y, breeze)).
no_pitD3(MaxX, MaxY, X,Y) :- NewY is Y-1, NewY > 0, NewY =< MaxY, board(X,NewY,visited), not(board(X, NewY, breeze)).
no_pitD4(MaxX, MaxY, X,Y) :- NewY is Y+1, NewY > 0, NewY =< MaxY, board(X,NewY,visited), not(board(X, NewY, breeze)).	

update_state(Action, Knowledge) :- myPosition(X,Y,Orient), assert(board(X,Y,free)), assert(board(X,Y,visited)), retract(board(X,Y, not_visited)), 
									(
										(stench, assert(board(X,Y,stench)));
										(breeze, assert(board(X,Y,breeze)))
									), fail.

in_board(X,Y) :- myWorldSize(MaxX, MaxY), X > 0, Y > 0, X =< MaxX, Y =< MaxY.

safe_pos(X,Y) :- in_board(X,Y), no_pit(X,Y), no_wumpus(X,Y).

setup_safe(Knowledge) :- board(X,Y,not_visited), safe_pos(X,Y), assert(board(X,Y,safe)), fail.

% standard action generating rules
% this is our agent's algorithm, the rules will be tried in order
act(Action, Knowledge) :- update_state(Action, Knowledge).
%act(Action, Knowledge) :- setup_safe(Knowledge).
act(Action, Knowledge) :- exit_if_home(Action, Knowledge). %if at home with gold
act(Action, Knowledge) :- pick_up_gold(Action, Knowledge). %if just found gold
act(Action, Knowledge) :- ((achieved_goal(1, 1), find_new_goal(1, 1));true), pursue_goal(Action, Knowledge).
%act(Action, Knowledge) :- ((achieved_goal(1, 1), find_new_goal(1, 1));true), Action=turnLeft, sum_knowledge(Knowledge).

find_new_goal(Action, Knowledge) :-	find_safe_not_visited_position(NewGoalX, NewGoalY), myPosition(X,Y, Orient),
									 currentGoal([GoalX, GoalY]), retract(currentGoal([GoalX, GoalY])),	assert(currentGoal([NewGoalX, NewGoalY])),
									 currentTabu(Tabu), retract(currentTabu(Tabu)), assert(currentTabu([[X,Y]])).

achieved_goal(Action, Knowledge) :- currentGoal([GoalX, GoalY]), myPosition(X,Y, Orient), X = GoalX, Y = GoalY.
find_safe_not_visited_position(X, Y) :- board(X, Y, not_visited), safe_pos(X, Y).

pursue_goal(Action, Knowledge) :- currentTabu(Tabu),currentGoal([GoalX, GoalY]),myPosition(X,Y, Orient), going(FP), retract(going(FP)),
								(

									(is_right_good(X,Y,GoalX,GoalY, Tabu, FoundedPath), pursue_right(Action, Orient, X, Y, NewX, NewY, NewOrient), rev(FoundedPath,NFP), assert(going(NFP)));
									(is_left_good(X,Y,GoalX,GoalY, Tabu, FoundedPath), pursue_left(Action, Orient, X, Y, NewX, NewY, NewOrient),  rev(FoundedPath,NFP),assert(going(NFP)) );
									(is_up_good(X,Y,GoalX,GoalY, Tabu, FoundedPath), pursue_up(Action, Orient, X, Y, NewX, NewY, NewOrient), rev(FoundedPath,NFP), assert(going(NFP)));
									(is_down_good(X,Y,GoalX,GoalY, Tabu, FoundedPath), pursue_down(Action, Orient, X, Y, NewX, NewY, NewOrient), rev(FoundedPath,NFP), assert(going(NFP)))

										), 
									retract(myPosition(X,Y,Orient)), assert(myPosition(NewX, NewY, NewOrient)), 
									retract(currentTabu(Tabu)), NewTabu = [[NewX, NewY] | Tabu], assert(currentTabu(NewTabu)), 
											sum_knowledge(Knowledge).

pursue_left(Action, west, X, Y, NewX, NewY, NewOrient) :- Action = moveForward, NewX is X - 1, NewY = Y, NewOrient = west.
pursue_left(Action, east, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = north.
pursue_left(Action, north, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = west.
pursue_left(Action, south, X, Y, NewX, NewY, NewOrient) :- Action = turnRight, NewX = X, NewY = Y, NewOrient = west.

pursue_right(Action, east, X, Y, NewX, NewY, NewOrient) :- Action = moveForward, NewX is X + 1, NewY = Y, NewOrient = east.
pursue_right(Action, west, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = south.
pursue_right(Action, north, X, Y, NewX, NewY, NewOrient) :- Action = turnRight, NewX = X, NewY = Y, NewOrient = east.
pursue_right(Action, south, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = east.

pursue_down(Action, south, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY is Y - 1, NewOrient = south.
pursue_down(Action, north, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = west.
pursue_down(Action, east, X, Y, NewX, NewY, NewOrient) :- Action = turnRight, NewX = X, NewY = Y, NewOrient = south.
pursue_down(Action, west, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = south.

pursue_up(Action, north, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY is Y + 1, NewOrient = north.
pursue_up(Action, south, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = east.
pursue_up(Action, east, X, Y, NewX, NewY, NewOrient) :- Action = turnLeft, NewX = X, NewY = Y, NewOrient = north.
pursue_up(Action, west, X, Y, NewX, NewY, NewOrient) :- Action = turnRight, NewX = X, NewY = Y, NewOrient = north.


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
	currentGoal(Goal),
	retract(currentGoal(Goal)),
	assert(currentGoal([1,1])),
	retract(nothing_to_do_here(false)),
	assert(nothing_to_do_here(true)),
	sum_knowledge(Knowledge).

can_reach(FromX, FromY, ToX, ToY, Tabu, FoundedPath) :- (FromX = ToX, FromY = ToY, FoundedPath = Tabu);(
												is_left_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
												is_down_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
												is_right_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath);
												is_up_good(FromX, FromY, ToX, ToY, Tabu, FoundedPath)
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