#include "bsg.h"
#include "bsgMenagerie.h"

#include <api/MinVR.h>

//TO DO:
//Ask about fake headtracker
//Scale cube and any additional viewpoint changes
//Lines->faces

//Determine best way to draw planes, map plane into triangles based off input

class drawableCompound4D;


class invisibleObj4D {
//Purpose: Contains the coordinates for a 4D Object and projects them onto
//a 3D object

private:

std::vector<glm::vec4> _v;



public:
/*Need to decide the types of each coordinate vertex
   How would I go from predefined vertices to getting functions->shapes
   Ex: Given the input: r cos 0, r sin 0, r^2 cos 2 0, r^2 sin 2 0), domain could
   be (-1,1), changes in height creates surfaces*/

invisibleObj4D(std::vector<glm::vec4> v
               ){
        _v = v;

}


// I am going to pass in three args, with index numbers indicating
// the coordinates I want to be returned in the result.  Let the X
// coordinate be zero, the Y is 1, the S is 2, and the T is 3.
std::vector<glm::vec4> projection(int one, int two, int three) {
        std::vector<glm::vec4> out;
        for (std::vector<glm::vec4>::iterator it = _v.begin();
             it != _v.end(); it++) {
                out.push_back(glm::vec4((*it)[one], (*it)[two], (*it)[three],1.0));
        }
        return out;
}



//setOrientation is a method that applies a rotationMatrix to the list of vertices,
//the rotationMatrix applied is determined based off three paramters, an angle
//to determine how much to rotate, and two ints that determine which the cos and sin locations within
//the matrix
void setOrientation(float angle, int numOne, int numTwo){
        glm::mat4 rotation = glm::mat4();
        rotation[numOne][numOne] = cos(angle);
        rotation[numOne][numTwo] = -sin(angle);
        rotation[numTwo][numOne] = sin(angle);
        rotation[numTwo][numTwo] = cos(angle);

        std::vector<glm::vec4> out;
        for (std::vector<glm::vec4>::iterator it = _v.begin();
             it != _v.end(); it++) {
                //*it= rotation * (*it);
                out.push_back(glm::vec4(rotation * (*it)));
        }
        _v = out;


}
std::vector<glm::vec4> getVertices(){
    return _v;
}
};




class drawableCompound4D : public bsg::drawableCompound {
//Purpose: Adds 4D ability to drawableCompound
private:
invisibleObj4D* _inviz;
int _one, _two, _three;
bsg::bsgPtr<bsg::drawableObj> _obj;




public:
drawableCompound4D(bsg::bsgPtr<bsg::shaderMgr> shader, const std::string name, invisibleObj4D
                   * inviz, const int one,const int two,const int three,  std::vector<glm::vec4> colors) :
        drawableCompound(name, shader) {
        _inviz = inviz;
        _one = one;
        _two = two;
        _three = three;
        _obj = new bsg::drawableObj();
        _obj->addData(bsg::GLDATA_VERTICES, "position", _inviz->projection(_one,_two,_three));
        _obj->addData(bsg::GLDATA_COLORS, "color", colors);
        _obj->setDrawType(GL_LINES);
        this->addObject(_obj);
}
void load(invisibleObj4D* invisible){
        _pShader->useProgram();
        _pShader->load();
        _totalModelMatrix = getModelMatrix();

        //Takes in the first item of the list and sets data to 3 coords out of 4
        //from the drawableObj
        _obj->setData(bsg::GLDATA_VERTICES, invisible->projection(_one,_two,_three));

}
};

class drawable4DFunction : public drawableCompound4D{
private:
float _numOne;
float _numTwo;
float _numThree;
float _numFour;

public:

};


class drawableCompound4DCube {
private:

drawableCompound4D* _cubeSetOne;
drawableCompound4D* _cubeSetTwo;
drawableCompound4D* _cubeSetThree;
drawableCompound4D* _cubeSetFour;
bsg::drawableCollection* _cubeCollection;

invisibleObj4D* _inviz;

public:
drawableCompound4DCube(bsg::bsgPtr<bsg::shaderMgr> shader, const std::string name
                       )  {

        std::vector<glm::vec4> cubeVertices;
        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        std::vector<glm::vec4> cubeColors;

        cubeColors.push_back(glm::vec4( .5f, 1.0f, .5f, 1.0f));
        cubeColors.push_back(glm::vec4( .5f, 1.0f, .5f, 1.0f));
        cubeColors.push_back(glm::vec4( .5f, 1.0f, .5f, 1.0f));

        cubeColors.push_back(glm::vec4( 1.0f, .5f, 1.0f, 1.0f));
        cubeColors.push_back(glm::vec4( .2f, .5f, 1.0f, 1.0f));
        cubeColors.push_back(glm::vec4( 1.0f, .5f, 1.0f, 1.0f));

        cubeColors.push_back(glm::vec4( .3, 1.0f, .5f, 1.0f));
        cubeColors.push_back(glm::vec4( 1.0f, .3f, .8f, 1.0f));
        cubeColors.push_back(glm::vec4( 1.0f, 1.0f, .5f, 1.0f));

        cubeColors.push_back(glm::vec4( .2, 1.0f, .1f, 1.0f));
        cubeColors.push_back(glm::vec4( .2, 1.0f, .1f, 1.0f));
        cubeColors.push_back(glm::vec4( .2, 1.0f, .1f, 1.0f));

        cubeColors.push_back(glm::vec4( .1, .7f, .2f, 1.0f));
        cubeColors.push_back(glm::vec4( .1, .7f, .2f, 1.0f));
        cubeColors.push_back(glm::vec4( .4, .7f, .2f, 1.0f));

        cubeColors.push_back(glm::vec4( 0.0f, .4f, 0.0f, 1.0f));
        cubeColors.push_back(glm::vec4( .5, .2f, .23f, 1.0f));
        cubeColors.push_back(glm::vec4( 0.0f, .3f, 0.0f, 1.0f));

        cubeColors.push_back(glm::vec4( 0.0f, .2f, 0.0f, 1.0f));
        cubeColors.push_back(glm::vec4( .5, .2f, .23f, 1.0f));
        cubeColors.push_back(glm::vec4( 0.4f, .85f, 0.0f, 1.0f));

        cubeColors.push_back(glm::vec4( 0.1f, .2f, 0.3f, 1.0f));
        cubeColors.push_back(glm::vec4( .4, .5f, .6f, 1.0f));
        cubeColors.push_back(glm::vec4( 0.7f, .8f, 0.9f, 1.0f));

        cubeColors.push_back(glm::vec4( 0.1f, .9f, 0.9f, 1.0f));
        cubeColors.push_back(glm::vec4( .2, .2f, .2f, 1.0f));
        cubeColors.push_back(glm::vec4( 0.7f, .82f, 0.9f, 1.0f));

        cubeColors.push_back(glm::vec4( 0.6f, .0f, 0.1f, 1.0f));
        cubeColors.push_back(glm::vec4( .4, .0f, .0f, 1.0f));
        cubeColors.push_back(glm::vec4( 0.7f, .2f, 0.9f, 1.0f));

        cubeColors.push_back(glm::vec4( 0.33, 0.1, 0.11, 1.0f));
        cubeColors.push_back(glm::vec4( 0.33, 0.25, 0.91, 1.0f));
        cubeColors.push_back(glm::vec4( 0.33, 0.95, 0.91, 1.0f));

        cubeColors.push_back(glm::vec4( 0.35, 0.51, 0.87, 1.0f));
        cubeColors.push_back(glm::vec4( 0.35, 0.51, 0.87, 1.0f));
        cubeColors.push_back(glm::vec4( 0.35, 0.51, 0.87, 1.0f));


        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));
        cubeColors.push_back(glm::vec4( 0.70, 0.70, 0.70, 1.0f));

        _inviz = new invisibleObj4D(cubeVertices);

        _cubeSetOne = new drawableCompound4D(shader, "cubeOne", _inviz, 1, 2, 3, cubeColors);
        _cubeSetTwo = new drawableCompound4D(shader, "cubeTwo", _inviz, 0, 2, 3, cubeColors);
        _cubeSetThree = new drawableCompound4D(shader, "cubeThree", _inviz, 0, 1, 3, cubeColors);
        _cubeSetFour = new drawableCompound4D(shader, "cubeFour", _inviz, 0, 1, 2, cubeColors);
        _cubeCollection = new bsg::drawableCollection("cubes");

        _cubeCollection->addObject(_cubeSetOne);
        _cubeCollection->addObject(_cubeSetTwo);
        _cubeCollection->addObject(_cubeSetThree);
        _cubeCollection->addObject(_cubeSetFour);


          _cubeSetOne->setPosition(0.0f,5.0f,0.0f);
          _cubeSetTwo->setPosition(-4.0f, 0.0f, 0.0f);
          _cubeSetThree->setPosition(4.0f, 0.0f, 0.0f);
          _cubeSetFour->setPosition(0.0f, -5.0, 0.0f);



}
bsg::drawableCollection* getCubes(){
        return _cubeCollection;
}
invisibleObj4D* getInvisObj(){
    return _inviz;
}

void updateCubes(){
    _cubeSetOne->load(_inviz);
    _cubeSetTwo->load(_inviz);
    _cubeSetThree->load(_inviz);
    _cubeSetFour->load(_inviz);
}
};
class DemoVRApp : public MinVR::VRApp {

// Data values that were global in the demo2.cpp file are defined as
// private members of the VRApp.
private:

// The scene and the objects in it must be available from the main()
// function where it is created and the renderScene() function where
// it is drawn.  The scene object contains all the drawable objects
// that make up the scene.
bsg::scene _scene;

// These are the shapes that make up the scene.  They are out here
// in the variables global to this object so they can be available
// in both the run() function and the renderScene() function.
drawableCompound4DCube* _cube;
bsg::drawableCollection* _cubes;
bsg::drawableCompound* _axesSet;


// These are part of the animation stuff, and again are out here with
// the big boy global variables so they can be available to both the
// interrupt handler and the render function.
float _oscillator;
float _oscillationStep;


// These variables were not global before, but their scope has been
// divided into several functions here, so they are class-wide
// private data objects.
bsg::bsgPtr<bsg::shaderMgr> _shader;
bsg::bsgPtr<bsg::lightList> _lights;

// This is the drawable object that makes up the compound _axesSet
// object.  There is a drawableAxes object in the bsg menagerie, but
// we are doing it this way for illustration.
bsg::bsgPtr<bsg::drawableObj> _axes;


std::string _vertexFile;
std::string _fragmentFile;


// These functions from demo2.cpp are not needed here:
//
//    init()
//    makeWindow()
//    resizeWindow()
//    ... also most of the processKeys() methods.
//
// The functionality of these methods is assumed by the MinVR apparatus.

// This contains a bunch of sanity checks from the graphics
// initialization of demo2.cpp.  They are still useful with MinVR.
void _checkContext() {

        // There is one more graphics library used here, called GLEW.  This
        // library sorts through the various OpenGL updates and changes and
        // allows a user to pretend that it's all a consistent and simple
        // system.  The 'core profile' refers to some modern OpenGL
        // enhancements that are not so modern as of 2017.  Set this to true
        // to get those enhancements.
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK) {
                throw std::runtime_error("Failed to initialize GLEW");
        }

        // Now that we have a graphics context, let's look at what's inside.
        std::cout << "Hardware check: "
                  << glGetString(GL_RENDERER) // e.g. Intel 3000 OpenGL Engine
                  << " / "
                  << glGetString(GL_VERSION) // e.g. 3.2 INTEL-8.0.61
                  << std::endl;

        if (glewIsSupported("GL_VERSION_2_1")) {
                std::cout << "Software check: Ready for OpenGL 2.1." << std::endl;
        } else {
                throw std::runtime_error("Software check: OpenGL 2.1 not supported.");
        }

        // This is the background color of the viewport.
        glClearColor(0.1, 0.0, 0.4, 1.0);

        // Now we're ready to start issuing OpenGL calls.  Start by enabling
        // the modes we want.  The DEPTH_TEST is how you get hidden faces.
        glEnable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        if (glIsEnabled(GL_DEPTH_TEST)) {
                std::cout << "Depth test enabled" << std::endl;
        } else {
                std::cout << "No depth test enabled" << std::endl;
        }

        // This is just a performance enhancement that allows OpenGL to
        // ignore faces that are facing away from the camera.


}

// Just a little debug function so that a user can see what's going on
// in a non-graphical sense.
void _showCameraPosition() {

        std::cout << "Camera is at ("
                  << _scene.getCameraPosition().x << ", "
                  << _scene.getCameraPosition().y << ", "
                  << _scene.getCameraPosition().z << ")... ";
        std::cout << "looking at ("
                  << _scene.getLookAtPosition().x << ", "
                  << _scene.getLookAtPosition().y << ", "
                  << _scene.getLookAtPosition().z << ")." << std::endl;
}
void _initScene(){

        _lights->addLight(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                          glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
        // _lights->addLight(glm::vec4(0.0f, 0.0f,-1.0f, 1.0f),
        //                   glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

        // Create a shader manager and load the light list.
        _shader->addLights(_lights);

        // Add the shaders to the manager, first the vertex shader...
        _shader->addShader(bsg::GLSHADER_VERTEX, _vertexFile);

        // ... then the fragment shader.  You could potentially add a
        // geometry shader at this point.
        _shader->addShader(bsg::GLSHADER_FRAGMENT, _fragmentFile);

        // The shaders are loaded, now compile them.
        _shader->compileShaders();

        // Now let's add a set of axes.
        _axes = new bsg::drawableObj();
        std::vector<glm::vec4> axesVertices;
        axesVertices.push_back(glm::vec4( -100.0f, 0.0f, 0.0f, 1.0f));
        axesVertices.push_back(glm::vec4( 100.0f, 0.0f, 0.0f, 1.0f));

        axesVertices.push_back(glm::vec4( 0.0f, -100.0f, 0.0f, 1.0f));
        axesVertices.push_back(glm::vec4( 0.0f, 100.0f, 0.0f, 1.0f));

        axesVertices.push_back(glm::vec4( 0.0f, 0.0f, -100.0f, 1.0f));
        axesVertices.push_back(glm::vec4( 0.0f, 0.0f, 100.0f, 1.0f));

        _axes->addData(bsg::GLDATA_VERTICES, "position", axesVertices);

        // With colors. (X = red, Y = green, Z = blue)
        std::vector<glm::vec4> axesColors;
        axesColors.push_back(glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f));
        axesColors.push_back(glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f));

        axesColors.push_back(glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f));
        axesColors.push_back(glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f));

        axesColors.push_back(glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f));
        axesColors.push_back(glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f));

        _axes->addData(bsg::GLDATA_COLORS, "color", axesColors);

        // The axes are not triangles, but lines.
        _axes->setDrawType(GL_LINES);

        std::vector<glm::vec4> vertices;


        _cube = new drawableCompound4DCube(_shader, "cube");
        _cubes = (_cube->getCubes());
        _axesSet = new bsg::drawableCompound(_shader);
        _axesSet->addObject(_axes);

        _scene.addObject(_axesSet);
        _scene.addObject(_cubes);
}



public:
DemoVRApp(int argc, char** argv) :
        MinVR::VRApp(argc, argv) {

        // This is the root of the scene graph.
        bsg::scene _scene = bsg::scene();

        // These are tracked separately because multiple objects might use
        // them.
        _shader = new bsg::shaderMgr();
        _lights = new bsg::lightList();

        _oscillator = 4.0f;
        _vertexFile = std::string(argv[1]);
        _fragmentFile = std::string(argv[2]);

}

/// The MinVR apparatus invokes this method whenever there is a new
/// event to process.
void onVREvent(const MinVR::VREvent &event) {

        //event.print();

        // This heartbeat event recurs at regular intervals, so you can do
        // animation with the model matrix here, as well as in the render
        // function.
        // if (event.getName() == "FrameStart") {
        //   const double time = event.getDataAsDouble("ElapsedSeconds");
        //   return;
        // }

        //float step = 0.5f;
        //float stepAngle = 5.0f / 360.0f;

        // Quit if the escape button is pressed
        if (event.getName() == "KbdEsc_Down") {
                shutdown();
        } else if (event.getName() == "FrameStart") {
                _oscillator = event.getDataAsFloat("ElapsedSeconds");
        }

        // Print out where you are (where the camera is) and where you're
        // looking.
        // _showCameraPosition();

}

/// \brief Set the render context.
///
/// The onVRRender methods are the heart of the MinVR rendering
/// apparatus.  Some render calls are shared among multiple views,
/// for example a stereo view has two renders, with the same render
/// context.
void onVRRenderGraphicsContext(const MinVR::VRGraphicsState &renderState) {

        // Check if this is the first call.  If so, do some initialization.
        if (renderState.isInitialRenderCall()) {
                _checkContext();
                _initScene();
                _scene.prepare();
        }
}

/// This is the heart of any graphics program, the render function.
/// It is called each time through the main graphics loop, and
/// re-draws the scene according to whatever has changed since the
/// last time it was drawn.
void onVRRenderGraphics(const MinVR::VRGraphicsState &renderState) {
        // Only draw if the application is still running.
        if (isRunning()) {
                // If you want to adjust the positions of the various objects in
                // your scene, you can do that here.
                _oscillationStep = .005f;
                float angle;
                angle = 0.01;
                //Oscillator goes here

                /*glm::vec3 cubePos = _cubes->getPosition();
               _oscillator += _oscillationStep;
               cubePos.x = cos(_oscillator);
               cubePos.y = 1.0f - cos(_oscillator);
               _cubes->setPosition(cubePos);*/

               //_oscillator += _oscillationStep;
               angle+= angle;
                _cube->getInvisObj()->setOrientation(angle,1,2);
                _cube->updateCubes();



                // Now the preliminaries are done, on to the actual drawing.

                // First clear the display.
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                // Second the load() step.  We let MinVR give us the projection
                // matrix from the render state argument to this method.
                const float* pm = renderState.getProjectionMatrix();
                glm::mat4 projMatrix = glm::mat4( pm[0],  pm[1], pm[2], pm[3],
                                                  pm[4],  pm[5], pm[6], pm[7],
                                                  pm[8],  pm[9],pm[10],pm[11],
                                                  pm[12],pm[13],pm[14],pm[15]);
                //bsg::bsgUtils::printMat("proj", projMatrix);
                _scene.load();

                // The draw step.  We let MinVR give us the view matrix.
                const float* vm = renderState.getViewMatrix();
                glm::mat4 viewMatrix = glm::mat4( vm[0],  vm[1], vm[2], vm[3],
                                                  vm[4],  vm[5], vm[6], vm[7],
                                                  vm[8],  vm[9],vm[10],vm[11],
                                                  vm[12],vm[13],vm[14],vm[15]);

                //bsg::bsgUtils::printMat("view", viewMatrix);
                _scene.draw(viewMatrix, projMatrix);

                //_scene.draw(_scene.getViewMatrix(), _scene.getProjMatrix());

                // We let MinVR swap the graphics buffers.
                // glutSwapBuffers();
        }
}

};

// The main function is just a shell of its former self.  Just
// initializes a MinVR graphics object and runs it.
int main(int argc, char **argv) {

        // Let's see what arguments the user invoked this program with.
        std::cout << "Invoked with argc=" << argc << " arguments." << std::endl;
        for (int i = 0; i < argc; i++) {
                std::cout << "argv[" << i << "]: " << std::string(argv[i]) << std::endl;
        }

        // If there weren't enough args, throw an error and explain what the
        // user should have done.
        if (argc < 4) {
                throw std::runtime_error("\nNeed three args, including the names of a vertex and fragment shader.\nTry 'bin/mathDemo ../shaders/shader2.vp ../shaders/shader.fp -c ../config/desktop-freeglut.xml'");
        }

        // Is the MINVR_ROOT variable set?  MinVR usually needs this to find
        // some important things.
        if (getenv("MINVR_ROOT") == NULL) {
                std::cout << "***** No MINVR_ROOT -- MinVR might not be found *****" << std::endl
                          << "MinVR is found (at runtime) via the 'MINVR_ROOT' variable."
                          << std::endl << "Try 'export MINVR_ROOT=/my/path/to/MinVR'."
                          << std::endl;
        }

        // Initialize the app.
        DemoVRApp app(argc, argv);

        // Run it.
        app.run();

        // We never get here.
        return 0;
}
