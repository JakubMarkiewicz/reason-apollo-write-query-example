module ReadChatQuery = ApolloClient.ReadQuery(GetDraftsQuery.GetDrafts);
module WriteChatQuery = ApolloClient.WriteQuery(GetDraftsQuery.GetDrafts);

let newDraft = (): GetDraftsQuery.draft => {
  "id": BsFaker.Random.uuid(),
  "published": false,
  "title": BsFaker.Random.words(~count=2, ()),
  "content": BsFaker.Random.words(~count=4, ()),
  "__typename": "Post",
};

let renderData = data =>
  data
  |> Array.map((v: GetDraftsQuery.draft) =>
       <div style={ReactDOMRe.Style.make(~display="flex", ())}>
         <div style={ReactDOMRe.Style.make(~marginRight="20px", ())}>
           {"id: " ++ v##id |> React.string}
         </div>
         <div style={ReactDOMRe.Style.make(~marginRight="20px", ())}>
           {"title: " ++ v##title |> React.string}
         </div>
         <div style={ReactDOMRe.Style.make(~marginRight="20px", ())}>
           {"content: " ++ v##content |> React.string}
         </div>
       </div>
     )
  |> React.array;

let writeToCache = client => {
  switch (ReadChatQuery.make(~client, ())) {
  | Some(data) =>
    WriteChatQuery.make(
      ~client,
      ~data={
        "drafts": Array.concat([data##drafts, [|newDraft()|]]),
        "__typename": "Query",
      },
      (),
    )
  | None => Js.log("no")
  };
};

[@react.component]
let make = () =>
  <ApolloConsumer>
    {client =>
       <GetDraftsQuery>
         {data =>
            <div>
              <button onClick={_ => writeToCache(client)}>
                {"Write new data" |> React.string}
              </button>
              {renderData(data)}
            </div>}
       </GetDraftsQuery>}
  </ApolloConsumer>;