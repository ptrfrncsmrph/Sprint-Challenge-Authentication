module MyAppStatus = {
  let isUserLoggedIn = true;
};

let str = ReasonReact.string;

module Link = {
  let component = ReasonReact.statelessComponent("Link");
  let make = (~href, ~className="", children) => {
    ...component,
    render: self =>
      <a
        href
        className
        onClick={
          self.handle((event, _self) => {
            ReactEvent.Mouse.preventDefault(event);
            ReasonReact.Router.push(href);
          })
        }>
        ...children
      </a>,
  };
};

type route =
  | Home
  | Signup
  | Login
  | Jokes;

type state = {route};

type action =
  | ChangeRoute(route);

let component = ReasonReact.reducerComponent("App");
let make = _children => {
  ...component,
  initialState: () => {route: Home},

  reducer: (action, _state) =>
    switch (action) {
    | ChangeRoute(route) => ReasonReact.Update({route: route})
    },

  didMount: self => {
    let watcherID =
      ReasonReact.Router.watchUrl(url =>
        switch (url.path, MyAppStatus.isUserLoggedIn) {
        | ([], _) => self.send(ChangeRoute(Home))
        | (_, true) => self.send(ChangeRoute(Jokes))
        | (["jokes"], false) => self.send(ChangeRoute(Login))
        | (["login"], false) => self.send(ChangeRoute(Login))
        | (["signup"], false) => self.send(ChangeRoute(Signup))
        | _ => self.send(ChangeRoute(Home))
        }
      );
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },

  render: ({state}) => {
    <>
      <nav> <Link href="/jokes"> {"Jokes" |> str} </Link> </nav>
      {switch (state.route) {
       | Home => <Home message="Hello, from App" />
       | Jokes => <Home message="JOKES   !!!! Hellowjadjfas;djf" />
       | _ => <Home message="Hellowjadjfas;djf" />
       }}
    </>;
  },
};