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

type token = string;

type loggedIn =
  | None
  | Some(token);

type state = {
  route,
  loggedIn,
};

type action =
  | ChangeRoute(route)
  | LogIn(token);

let component = ReasonReact.reducerComponent("App");
let make = _children => {
  ...component,
  initialState: () => {route: Home, loggedIn: None},

  reducer: (action, state) =>
    switch (action) {
    | ChangeRoute(route) => ReasonReact.Update({...state, route})
    | LogIn(token) => ReasonReact.Update({...state, loggedIn: Some(token)})
    },

  didMount: self => {
    let watcherID =
      ReasonReact.Router.watchUrl(url =>
        switch (url.path, MyAppStatus.isUserLoggedIn) {
        | ([], _) => self.send(ChangeRoute(Home))
        | (_, true) => self.send(ChangeRoute(Jokes))
        | (["jokes"], false) => self.send(ChangeRoute(Login))
        | (["login"], false) => self.send(ChangeRoute(Login))
        | (["logout"], false) => self.send(ChangeRoute(Login))
        | (["signup"], false) => self.send(ChangeRoute(Signup))
        | _ => self.send(ChangeRoute(Home))
        }
      );
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherID));
  },

  render: ({state}) => {
    <>
      <nav>
        <Link href="/"> {"Home" |> str} </Link>
        <Link href="/jokes"> {"Jokes" |> str} </Link>
        {MyAppStatus.isUserLoggedIn
           ? <Link href="/logout"> {"Logout" |> str} </Link>
           : <Link href="/login"> {"Login" |> str} </Link>}
        <Link href="/signup"> {"Signup" |> str} </Link>
      </nav>
      {switch (state.route) {
       | Home => <Home />
       | Jokes => <Jokes />
       | _ => <Home />
       }}
    </>;
  },
};