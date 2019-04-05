let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Jokes");

let make = _children => {
  ...component,
  render: _self => <h1> {"Jokes" |> str} </h1>,
};