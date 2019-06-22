/* Create a GraphQL Query by using the graphql_ppx */
type draft = {
  .
  "id": string,
  "published": bool,
  "title": string,
  "content": string,
  "__typename": string,
};

module GetDrafts = [%graphql
  {|
  query getDrafts {
  __typename
  drafts {
    id
    published
    title
    content
    __typename
  }
}
|}
];

module GetDraftsQuery = ReasonApollo.CreateQuery(GetDrafts);

[@react.component]
let make = (~children) =>
  <GetDraftsQuery>
    ...{({result}) =>
      switch (result) {
      | Loading => <div> {ReasonReact.string("Loading")} </div>
      | Error(error) => <div> {ReasonReact.string(error##message)} </div>
      | Data(response) => children(response##drafts)
      }
    }
  </GetDraftsQuery>;