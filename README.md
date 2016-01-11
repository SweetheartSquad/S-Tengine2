#S-Tengine2#

# Prerequisites 

* Visual Studio 2012 or 2013
* A computer with graphics card that supports OpenGL 3.3


# Set-up Instructions #
1. Dependencies:
 1. Download the [dev folder](https://drive.google.com/file/d/0B2VyAXziLJJtNXJ2Y0REU3RrSm8/view?usp=sharing) with all the necessary libraries
 2. Open dev/Box2D-2.3.1/Box2D/Build/vs2013/Box2D.sln and build
 3. Open dev/Bullet-2.83-alpha/build3/vs2010/0_Bullet3Solution.sln and build
2. Engine:
 1. Download or clone [S-Tengine2](https://github.com/SweetheartSquad/S-Tengine2)
 2. Rename S-Tengine2/Source/directories.props.template to directories.props
 3. Open S-Tengine2/Source/S-Tengine2.sln
 4. Edit the directories.props file so that each entry points to your installation location for the [dev folder](https://drive.google.com/file/d/0B2VyAXziLJJtNXJ2Y0REU3RrSm8/view?usp=sharing)
 5. Build
3. Template Project:
 1. Download or clone the [template project](https://github.com/SweetheartSquad/TemplateProject)
 2. Rename Game/Source/gameSettings.props.template to gameSettings.props
 3. Open Game/Source/Game.sln
 4. Edit the gameSettings.props file so that each entry points to your installation location for the [dev folder](https://drive.google.com/file/d/0B2VyAXziLJJtNXJ2Y0REU3RrSm8/view?usp=sharing), and the engine directory points to your installation location for S-Tengine2



## Individual Libraries (Manual Installation) ##

### GLEW ###
* Download glew 1.11.0 
* http://sourceforge.net/projects/glew/files/glew/1.11.0/glew-1.11.0-win32.zip/download

* Unzip the folder and place it in an accessible location. Set the GLEW_DIR property to this location.
* e.g. <GLEW_DIR>C:\Dev\glew-1.10.0</GLEW_DIR>
 
### GLFW ###
* Download GLFW 3.0.4
* http://sourceforge.net/projects/glfw/files/glfw/3.0.4/glfw-3.0.4.bin.WIN32.zip/download

* Unzip the folder and place it in an accessible location. Set the GLFW_DIR property to this location
* e.g. <GLFW_DIR> C:\Dev\glfw-3.0.4.bin.WIN32</GLFW_DIR>
 
### GLM ###
* Download GLM 0.9.5.4
* http://sourceforge.net/projects/ogl-math/files/glm-0.9.5.4/glm-0.9.5.4.zip/download

* Unzip the folder and place it in an accessible location. Set the GLM_DIR property to this location
* e.g. <GLM_DIR>C:\Dev\glm</GLM_DIR>

### Some more that we haven't added yet ###
- Box2D
- Bullet
- Freetype
- OpenAL Soft + libsndfile
- RestCpp