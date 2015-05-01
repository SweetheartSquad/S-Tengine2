#S-Tengine2#

# Prerequisites 

* Visual Studio 2012 or 2013
* A computer with graphics card that supports OpenGL 3.3


# Set-up Instructions #
1. Download the necessary libraries (links provided below)
2. Open solution file
3. Rename voxSettings.props.template to voxSettings.props
4. Change the various directory items in voxSettings.props to point to your local installation of each library


## Library Pack (Express Installation)  

https://drive.google.com/file/d/0B2VyAXziLJJtNXJ2Y0REU3RrSm8/view?usp=sharing

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
 
### SOIL ###
Download SOIL
http://www.lonesock.net/files/soil.zip

* Unzip the folder and place it in an accessible location. Set the SOIL_DIR property to this location
* e.g. <SOIL_DIR> C:\Dev\Simple OpenGL Image Library</SOIL_DIR>

* Open the projects folder under in the library’s root, then open the VC9 folder.

* Open the SOIL.sln file in Visual Studio. You will have to upgrade the project.

* Once it is open, click Build to build the lib file.

### Some more that we haven't added yet ###
Box2D

**The project should now build**


### Useful Resources ###
http://www.slideshare.net/Mark_Kilgard/using-vertex-bufferobjectswell
