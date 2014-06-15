%%%-------------------------------------------------------------------
%%% @author tjacek
%%% @copyright (C) 2014, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 09. kwi 2014 12:38
%%%-------------------------------------------------------------------
-module(regression).
-author("tjacek").

%% API
-export([run_exp/1,test_regression/2,parse_labels/1]).

run_exp(Args) ->
  TrainFile = lists:nth(1,Args),
  TestFile = lists:nth(2,Args),
  test_regression(TrainFile,TestFile).

test_regression(TrainFile,TestFile) ->
  io:format("OK \n"),
  {Attributes, TrainSet} = mllib:read(arff,[{file,TrainFile}]),
  {Attributes, TestSet} = mllib:read(arff,[{file,TestFile}]),
  {Y_true,X}=parse_labels(TestSet),
  Model=kernel_smoother:learn(TrainSet),
  Y_pred=kernel_smoother:apply_regression(X,Model),
  Error=squared_error(Y_true,Y_pred),
  io:format("~p~n",[Error]).

squared_error(X,Y) ->
  Div=utils:substract(X,Y),
  Square=fun(X) ->
    X*X
  end,
  S=lists:sum(lists:map(Square,Div)),
  math:sqrt(S)/float(length(Div)).

error(X,Y) ->
  Div=utils:substract(X,Y),
  lists:sum(Div)/float(length(Div)).

parse_labels(Samples) ->
  Extract_Labels=fun(Sample) ->
    N=size(Sample),
    element(N,Sample)
  end,
  Labels=lists:map(Extract_Labels,Samples),
  Instances =remove_pred_variable(Samples),
  {Labels,Instances}.

remove_pred_variable(Raw_samples) ->
  Extract_Instances=fun(Sample) ->
    ListSample=tuple_to_list(Sample) ,
    Label=lists:last(ListSample),
    lists:delete(Label,ListSample)
  end,
  lists:map(Extract_Instances,Raw_samples).
