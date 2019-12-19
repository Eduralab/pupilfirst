[@bs.config {jsx: 3}];

[%bs.raw {|require("./EvaluationCriterionEditor__Form.css")|}];

let str = React.string;

open EvaluationCriteriaEditor__Types;

type state = {
  name: string,
  description: string,
  maxGrade: int,
  passGrade: int,
  selectedGrade: int,
  gradesAndLabels: array(GradesAndLabels.t),
  saving: bool,
  dirty: bool,
};

module CreateEvaluationCriterionQuery = [%graphql
  {|
   mutation($name: String!, $courseId: ID!, $description: String!, $maxGrade: Int!, $passGrade: Int!, $gradesAndLabels: [GradeAndLabelInput!]!) {
     createEvaluationCriterion(courseId: $courseId, name: $name, description: $description, maxGrade: $maxGrade, passGrade: $passGrade, gradesAndLabels: $gradesAndLabels ) {
       evaluationCriterion {
         id
       }
     }
   }
   |}
];

module UpdateEvaluationCriterionQuery = [%graphql
  {|
   mutation($id: ID!, $description: String!, $name: String!, $gradesAndLabels: [GradeAndLabelInput!]!) {
    updateEvaluationCriterion(id: $id, name: $name, description: $description, gradesAndLabels: $gradesAndLabels){
       evaluationCriterion {
         id
       }
      }
   }
   |}
];

let formClasses = value =>
  value ? "drawer-right-form w-full opacity-50" : "drawer-right-form w-full";

let possibleGradeValues: list(int) = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

let gradeBarBulletClasses = (selected, passed, empty) => {
  let classes = selected ? " grade-bar__pointer--selected" : " ";
  if (empty) {
    classes ++ " grade-bar__pointer--pulse";
  } else {
    passed
      ? classes ++ " grade-bar__pointer--passed"
      : classes ++ " grade-bar__pointer--failed";
  };
};

let updateMaxGrade = (value, state, setState) =>
  if (value <= state.passGrade) {
    setState(state =>
      {...state, passGrade: 1, selectedGrade: value, maxGrade: value}
    );
  } else {
    setState(state => {...state, selectedGrade: value, maxGrade: value});
  };
let updatePassGrade = (value, setState) => {
  setState(state => {...state, passGrade: value});
};

let updateGradeLabel = (value, gradeAndLabel, state, setState) => {
  let updatedGradeAndLabel = GradesAndLabels.update(value, gradeAndLabel);
  let gradesAndLabels =
    state.gradesAndLabels
    |> Array.map(gl =>
         gl
         |> GradesAndLabels.grade
         == (updatedGradeAndLabel |> GradesAndLabels.grade)
           ? updatedGradeAndLabel : gl
       );
  setState(state => {...state, gradesAndLabels});
};

let updateEvaluationCriterion =
    (state, setState, updateCriterionCB, criterion) => {
  setState(state => {...state, saving: true});

  let jsGradeAndLabelArray =
    state.gradesAndLabels
    |> Js.Array.filter(gradesAndLabel =>
         gradesAndLabel |> GradesAndLabels.grade <= state.maxGrade
       )
    |> Array.map(gl => gl |> GradesAndLabels.asJsType);

  let updateCriterionQuery =
    UpdateEvaluationCriterionQuery.make(
      ~id=criterion |> EvaluationCriterion.id,
      ~name=state.name,
      ~description=state.description,
      ~gradesAndLabels=jsGradeAndLabelArray,
      (),
    );
  let response =
    updateCriterionQuery
    |> GraphqlQuery.sendQuery(AuthenticityToken.fromHead());

  response
  |> Js.Promise.then_(result => {
       Js.log(result##updateEvaluationCriterion##evaluationCriterion##id);
       setState(state => {...state, saving: false, dirty: false});
       Js.Promise.resolve();
     })
  |> ignore;
};

let createEvaluationCriterion = (state, setState, updateCriterionCB, courseId) => {
  setState(state => {...state, saving: true});

  let jsGradeAndLabelArray =
    state.gradesAndLabels
    |> Js.Array.filter(gradesAndLabel =>
         gradesAndLabel |> GradesAndLabels.grade <= state.maxGrade
       )
    |> Array.map(gl => gl |> GradesAndLabels.asJsType);

  let createCriterionQuery =
    CreateEvaluationCriterionQuery.make(
      ~name=state.name,
      ~description=state.description,
      ~maxGrade=state.maxGrade,
      ~passGrade=state.passGrade,
      ~courseId,
      ~gradesAndLabels=jsGradeAndLabelArray,
      (),
    );
  let response =
    createCriterionQuery
    |> GraphqlQuery.sendQuery(AuthenticityToken.fromHead());

  response
  |> Js.Promise.then_(result => {
       Js.log(result##createEvaluationCriterion##evaluationCriterion##id);
       setState(state => {...state, saving: false, dirty: false});
       Js.Promise.resolve();
     })
  |> ignore;
};

let updateName = (setState, value) => {
  setState(state => {...state, dirty: true, name: value});
};

let updateDescription = (setState, value) => {
  setState(state => {...state, dirty: true, description: value});
};

let saveDisabled = state => {
  let hasValidGrades =
    state.gradesAndLabels
    |> Js.Array.filter(gl => gl |> GradesAndLabels.grade <= state.maxGrade)
    |> Array.map(gl => gl |> GradesAndLabels.valid)
    |> Js.Array.filter(value => !value)
    |> ArrayUtils.isEmpty;
  let hasValidName = String.length(state.name) > 1;
  let hasValidDescription = String.length(state.description) > 1;
  !state.dirty
  || state.saving
  || !(hasValidGrades && hasValidName && hasValidDescription);
};

[@react.component]
let make = (~evaluationCriterion, ~courseId, ~updateCriterionCB=?) => {
  let (state, setState) =
    React.useState(() =>
      switch (evaluationCriterion) {
      | None => {
          name: "",
          description: "",
          maxGrade: 5,
          passGrade: 2,
          selectedGrade: 1,
          gradesAndLabels:
            possibleGradeValues
            |> List.map(i => GradesAndLabels.empty(i))
            |> Array.of_list,
          saving: false,
          dirty: false,
        }
      | Some(ec) => {
          name: ec |> EvaluationCriterion.name,
          description: ec |> EvaluationCriterion.description,
          maxGrade: ec |> EvaluationCriterion.maxGrade,
          passGrade: ec |> EvaluationCriterion.passGrade,
          selectedGrade: 1,
          gradesAndLabels: ec |> EvaluationCriterion.gradesAndLabels,
          saving: false,
          dirty: false,
        }
      }
    );
  <div className="mx-auto bg-white">
    <div className="max-w-2xl p-6 mx-auto">
      <h5 className="uppercase text-center border-b border-gray-400 pb-2">
        {(
           switch (evaluationCriterion) {
           | None => "Add Evaluation Criterion"
           | Some(ec) => ec |> EvaluationCriterion.name
           }
         )
         |> str}
      </h5>
      <DisablingCover
        disabled={state.saving}
        message={
          switch (evaluationCriterion) {
          | Some(_ec) => "Updating..."
          | None => "Saving..."
          }
        }>
        <div key="evaluation-criterion-editor" className="mt-3">
          <div className="mt-5">
            <label
              className="inline-block tracking-wide text-xs font-semibold "
              htmlFor="name">
              {"Name" |> str}
            </label>
            <input
              className="appearance-none block w-full bg-white border border-gray-400 rounded py-3 px-4 mt-2 leading-tight focus:outline-none focus:bg-white focus:border-gray-500"
              id="name"
              onChange={event =>
                updateName(setState, ReactEvent.Form.target(event)##value)
              }
              type_="text"
              placeholder="Type course name here"
              maxLength=50
              value={state.name}
            />
            <School__InputGroupError
              message="Enter a valid name"
              active={state.name |> String.length < 1}
            />
          </div>
          <div className="mt-5">
            <label
              className="inline-block tracking-wide text-xs font-semibold "
              htmlFor="name">
              {"Description" |> str}
            </label>
            <input
              className="appearance-none block w-full bg-white border border-gray-400 rounded py-3 px-4 mt-2 leading-tight focus:outline-none focus:bg-white focus:border-gray-500"
              id="description"
              type_="text"
              onChange={event =>
                updateDescription(
                  setState,
                  ReactEvent.Form.target(event)##value,
                )
              }
              placeholder="Type description for the evaluation criterion"
              maxLength=50
              value={state.description}
            />
            <School__InputGroupError
              message="Enter a valid description"
              active={state.description |> String.length < 1}
            />
          </div>
        </div>
        {<div className="mx-auto">
           <div className="max-w-2xl p-6 mx-auto">
             <h5
               className="uppercase text-center border-b border-gray-400 pb-2 mb-4">
               {"Grades" |> str}
             </h5>
             <div className="mb-4">
               <span
                 className="inline-block tracking-wide text-sm font-semibold mr-2"
                 htmlFor="max_grades">
                 {"Maximum grade is" |> str}
               </span>
               {switch (evaluationCriterion) {
                | Some(_) =>
                  <span
                    className="cursor-not-allowed inline-block bg-white border-b-2 text-2xl font-semibold text-center border-blue px-3 py-2 leading-tight rounded-none focus:outline-none">
                    {state.maxGrade |> string_of_int |> str}
                  </span>
                | None =>
                  <select
                    onChange={event =>
                      updateMaxGrade(
                        ReactEvent.Form.target(event)##value |> int_of_string,
                        state,
                        setState,
                      )
                    }
                    value={state.maxGrade |> string_of_int}
                    className="cursor-pointer inline-block appearance-none bg-white border-b-2 text-2xl font-semibold text-center border-blue hover:border-gray-500 px-3 py-2 leading-tight rounded-none focus:outline-none">
                    {possibleGradeValues
                     |> List.filter(g => g != 1)
                     |> List.map(possibleGradeValue =>
                          <option
                            key={possibleGradeValue |> string_of_int}
                            value={possibleGradeValue |> string_of_int}>
                            {possibleGradeValue |> string_of_int |> str}
                          </option>
                        )
                     |> Array.of_list
                     |> ReasonReact.array}
                  </select>
                }}
               <span
                 className="inline-block tracking-wide text-sm font-semibold mx-2"
                 htmlFor="pass_grades">
                 {"and the passing grade is" |> str}
               </span>
               {switch (evaluationCriterion) {
                | Some(_) =>
                  <span
                    className="cursor-not-allowed inline-block appearance-none bg-white border-b-2 text-2xl font-semibold text-center border-blue px-3 py-2 leading-tight rounded-none">
                    {state.passGrade |> string_of_int |> str}
                  </span>
                | None =>
                  <select
                    onChange={event =>
                      updatePassGrade(
                        ReactEvent.Form.target(event)##value |> int_of_string,
                        setState,
                      )
                    }
                    value={state.passGrade |> string_of_int}
                    className="cursor-pointer inline-block appearance-none bg-white border-b-2 text-2xl font-semibold text-center border-blue hover:border-gray-500 px-3 py-2 rounded-none leading-tight focus:outline-none">
                    {possibleGradeValues
                     |> List.filter(g => g < state.maxGrade)
                     |> List.map(possibleGradeValue =>
                          <option
                            key={possibleGradeValue |> string_of_int}
                            value={possibleGradeValue |> string_of_int}>
                            {possibleGradeValue |> string_of_int |> str}
                          </option>
                        )
                     |> Array.of_list
                     |> ReasonReact.array}
                  </select>
                }}
             </div>
             <label
               className="block tracking-wide text-xs font-semibold mb-2"
               htmlFor="grades">
               {"Grades" |> str}
             </label>
             <div className="flex">
               <div
                 className="flex flex-col bg-white p-6 shadow items-center justify-center rounded w-full">
                 <h2
                   className="grades__score-circle rounded-full h-24 w-24 flex items-center justify-center border-2 border-green-400 p-4 mb-4">
                   {(state.selectedGrade |> string_of_int)
                    ++ "/"
                    ++ (state.maxGrade |> string_of_int)
                    |> str}
                 </h2>
                 <div>
                   {state.gradesAndLabels
                    |> Js.Array.filter(gradeAndLabel =>
                         gradeAndLabel
                         |> GradesAndLabels.grade == state.selectedGrade
                       )
                    |> Array.map(gradeAndLabel =>
                         <div
                           key={
                             gradeAndLabel
                             |> GradesAndLabels.grade
                             |> string_of_int
                           }>
                           <input
                             className="text-center grades__label-input appearance-none inline-block bg-white border border-gray-400 rounded py-2 px-4 mb-6 leading-tight focus:outline-none focus:bg-white focus:border-gray-500"
                             id={
                               "label"
                               ++ (
                                 gradeAndLabel
                                 |> GradesAndLabels.grade
                                 |> string_of_int
                               )
                             }
                             type_="text"
                             placeholder="Type grade label"
                             value={gradeAndLabel |> GradesAndLabels.label}
                             onChange={event =>
                               updateGradeLabel(
                                 ReactEvent.Form.target(event)##value,
                                 gradeAndLabel,
                                 state,
                                 setState,
                               )
                             }
                           />
                         </div>
                       )
                    |> ReasonReact.array}
                 </div>
                 <div className="grade-bar__container w-full mb-6">
                   <ul className="grade-bar__track flex justify-between">
                     {state.gradesAndLabels
                      |> Js.Array.filter(gradesAndLabel =>
                           gradesAndLabel
                           |> GradesAndLabels.grade <= state.maxGrade
                         )
                      |> Array.map(gradesAndLabel =>
                           <li
                             key={
                               gradesAndLabel
                               |> GradesAndLabels.grade
                               |> string_of_int
                             }
                             className="flex flex-1 grade-bar__track-segment justify-center items-center relative"
                             onClick={_ =>
                               setState(state =>
                                 {
                                   ...state,
                                   selectedGrade:
                                     gradesAndLabel |> GradesAndLabels.grade,
                                 }
                               )
                             }>
                             <span
                               className="grade-bar__track-segment-title whitespace-no-wrap text-xs z-20">
                               {(
                                  gradesAndLabel |> GradesAndLabels.valid
                                    ? gradesAndLabel |> GradesAndLabels.label
                                    : "Add grade label"
                                )
                                |> str}
                             </span>
                             <label
                               htmlFor={
                                 "label"
                                 ++ (
                                   gradesAndLabel
                                   |> GradesAndLabels.grade
                                   |> string_of_int
                                 )
                               }
                               className={
                                 "flex items-center justify-center z-10 grade-bar__pointer"
                                 ++ gradeBarBulletClasses(
                                      gradesAndLabel
                                      |> GradesAndLabels.grade
                                      == state.selectedGrade,
                                      gradesAndLabel
                                      |> GradesAndLabels.grade
                                      >= state.passGrade,
                                      !(
                                        gradesAndLabel |> GradesAndLabels.valid
                                      ),
                                    )
                               }>
                               {gradesAndLabel
                                |> GradesAndLabels.grade
                                |> string_of_int
                                |> str}
                             </label>
                           </li>
                         )
                      |> ReasonReact.array}
                   </ul>
                 </div>
                 <div className="flex justify-between items-center pt-6 pb-5">
                   <div className="flex justify-center items-center mx-4">
                     <span
                       className="grade-bar__pointer-legend grade-bar__pointer-legend-failed"
                     />
                     <span className="ml-2 text-xs"> {"Fail" |> str} </span>
                   </div>
                   <div className="flex justify-center items-center mx-4">
                     <span
                       className="grade-bar__pointer-legend grade-bar__pointer-legend-passed"
                     />
                     <span className="ml-2 text-xs"> {"Passed" |> str} </span>
                   </div>
                   <div className="flex justify-center items-center mx-4">
                     <span
                       className="grade-bar__pointer-legend grade-bar__pointer--pulse"
                     />
                     <span className="ml-2 text-xs">
                       {"Add grade label" |> str}
                     </span>
                   </div>
                 </div>
               </div>
             </div>
             <div className="mt-3 mb-3 text-xs">
               <span className="leading-normal">
                 <strong> {"Important:" |> str} </strong>
                 {" The values for maximum and passing grades cannot be modified once a criterion is created. Labels given to each grade can be edited later on."
                  |> str}
               </span>
             </div>
             <div className="flex">
               {switch (evaluationCriterion) {
                | Some(criterion) =>
                  <button
                    disabled={saveDisabled(state)}
                    onClick={_ =>
                      updateEvaluationCriterion(
                        state,
                        setState,
                        updateCriterionCB,
                        criterion,
                      )
                    }
                    className="w-full btn btn-large btn-primary mt-3">
                    {"Update Criterion" |> str}
                  </button>

                | None =>
                  <button
                    disabled={saveDisabled(state)}
                    onClick={_ =>
                      createEvaluationCriterion(
                        state,
                        setState,
                        updateCriterionCB,
                        courseId,
                      )
                    }
                    className="w-full btn btn-large btn-primary mt-3">
                    {"Create Criterion" |> str}
                  </button>
                }}
             </div>
           </div>
         </div>}
      </DisablingCover>
    </div>
  </div>;
};
