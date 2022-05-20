# Material Creation From Json Plugin
https://user-images.githubusercontent.com/23010885/168930648-84226b13-02e0-44e2-9809-62acfbd91eb7.mp4


![material](https://user-images.githubusercontent.com/23010885/168932000-04c88bdc-7512-4ff5-9410-d4b0a5396542.PNG)

Plugin that through JSON file and the command "newmaterial path" executed in the console, where the path is the path of the json file
As of now not all the material option are implemented.
## Material Node Implemented
- Add
- Append Vector
- Component Mask
- Constant
- Constant 2 Vector
- Constant Bias Scale
- Custom Expression
- Divide
- Linear Interpolate
- Multiply
- Object Position
- One Minus
- Panner
- Power
- Runtime Virtual Texture Sample
- Saturate
- Scalar Parameter
- Sine
- Sphere Mask
- Subtract
- Time
- Transform Position
- Vector Parameter
- World Position

## Example
Example Json can be found in the folder named "MaterialJsonExample"

## Known Issues
- Some node don't support the entirety of possibility that you have in blueprint.
- For now the end attachment possible are only for the Base Color/Metallic/Emissive Color/Opacity Mask.

## Documentation
Documentation for the form of the json file can be found inside the same folder of the example

## UI
UI Window for the JsonMaterial plugin can be found under Window/Developer Tools/JsonEditorPlugin
![MatCreationFromJson - Unreal Editor 20_05_2022 19_06_34](https://user-images.githubusercontent.com/23010885/169580450-6c7a93c7-f9c9-4296-9dc6-a752f781b77f.png)


Open File Dialog to choose the Json
And Create Material to create the Json from the selected file
![pluginEditorDock](https://user-images.githubusercontent.com/23010885/169577492-54834cc6-5c78-4fc5-b4c3-a33c8394a7a4.PNG)

EXAMPLE:

https://user-images.githubusercontent.com/23010885/169579955-076e4346-a252-4328-9886-555ccd0a533d.mp4


## Information about this project
This is an exercise i made to improve my skill with unreal engine and c++, if anyone is interested in it and would like to have more node, feel free to contact me and tell me the node to add.
