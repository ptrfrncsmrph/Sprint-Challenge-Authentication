let component = ReasonReact.statelessComponent("Home");

let handleClick = (_event, _self) => {
  ReasonReact.Router.push("jokes");
};

let make = (~message, _children) => {
  ...component,
  render: self =>
    <div onClick={self.handle(handleClick)}>
      {ReasonReact.string(message)}
    </div>,
};