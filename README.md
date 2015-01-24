# Global Game Jam 2015 #

```
#!html

 __   __   ______     __  __    
/\ \ / /  /\  __ \   /\_\_\_\   
\ \ \'/   \ \ \/\ \  \/_/\_\/_  
 \ \__|    \ \_____\   /\_\/\_\ 
  \/_/      \/_____/   \/_/\/_/ 
                               
```


# Prerequisites 

* Visual Studio 2012
* A computer with graphics card that supports OpenGL 3.3

# Vox Set-up Instructions #

Open project

Rename voxSettings.props.template to voxSettings.props

## GLEW ##
* Download glew 1.11.0 
* http://sourceforge.net/projects/glew/files/glew/1.11.0/glew-1.11.0-win32.zip/download

* Unzip the folder and place it in an accessible location. Set the GLEW_DIR property to this location.
* e.g. <GLEW_DIR>C:\Dev\glew-1.10.0</GLEW_DIR>
 
## GLFW ##
* Download GLFW 3.0.4
* http://sourceforge.net/projects/glfw/files/glfw/3.0.4/glfw-3.0.4.bin.WIN32.zip/download

* Unzip the folder and place it in an accessible location. Set the GLFW_DIR property to this location
* e.g. <GLFW_DIR> C:\Dev\glfw-3.0.4.bin.WIN32</GLFW_DIR>
 
## GLM ##
* Download GLM 0.9.5.4
* http://sourceforge.net/projects/ogl-math/files/glm-0.9.5.4/glm-0.9.5.4.zip/download

* Unzip the folder and place it in an accessible location. Set the GLM_DIR property to this location
* e.g. <GLM_DIR>C:\Dev\glm</GLM_DIR>
 
## SOIL ##
Download SOIL
http://www.lonesock.net/files/soil.zip

* Unzip the folder and place it in an accessible location. Set the SOIL_DIR property to this location
* e.g. <SOIL_DIR> C:\Dev\Simple OpenGL Image Library</SOIL_DIR>

* Open the projects folder under in the library’s root, then open the VC9 folder.

* Open the SOIL.sln file in Visual Studio. You will have to upgrade the project.

* Once it is open, click Build to build the lib file.

**The project should now build**

# Vox Sample Project #

A sample project has been included as a zip file(SampleProject.zip) in the repository. This project is designed to run without any configuration. Simply open the project and click run. This will build and run the project. The project contains a sample scene which demonstrates the basic features of the Vox engine. These features include...

* Entities
* Shaders/Shader Components
* OBJ Model Loading
* JSON Model Loading
* Frame Buffers
* Render Surfaces
* Directional Lights
* Point Lights 
* Animations
* Shadows
* Camera
* Voxels
* Shader Loading
* Transformations

This sample project also has many comments which should clarify what is being done and why

## VOX library pack  

https://drive.google.com/file/d/0B2VyAXziLJJtNXJ2Y0REU3RrSm8/view?usp=sharing

=================================================================

## Icons ##
Icons used in the modelling software made by Amit Jakhu, Elegant Themes, Freepik, Icomoon from [www.flaticon.com](http://www.flaticon.com) is licensed by [Creative Commons 3.0](http://creativecommons.org/licenses/by/3.0/).

## Useful Links 

http://www.tomdalling.com/blog/category/modern-opengl/

http://www.informit.com/articles/article.aspx?p=461848

http://www.volumesoffun.com/polyvox/documentation/0.2.1/manual/index.html - an existing voxel engine's documentation, could give us ideas on handling our shader, textures, lighting. etc.