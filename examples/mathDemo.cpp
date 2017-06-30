#include "bsg.h"
#include "bsgMenagerie.h"

#include <api/MinVR.h>

class invisibleObj4D: public bsg::drawableCollection{

};

class drawableCompound4D : public bsg::drawableCompound{
private:
    std::vector<glm::vec4> _vertices;
public:
    //Smart Pointers auto keep track of how many times a variable is referenced
    void load() {


      return;


      // Load each component object.
    //   for (DrawableObjList::iterator it = _objects.begin();
    //        it != _objects.end(); it++) {
    //     (*it)->load();
      }
  };
class object4D {
private:
    drawableCompound4D sx;
    drawableCompound4D sy;
    drawableCompound4D ss;
    drawableCompound4D st;

    std::vector<glm::vec4> vertices;
public:
    std::vector<glm::vec3> getVertices(p){
        std::vector<glm::vec3> vec3;
        vec3.push_back
    }
};

class DemoVRApp: public MinVR::VRApp {

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

  bsg::drawableCompound* _axesSet;
  bsg::drawableCompound* _cubeOneSet;
  bsg::drawableCompound* _cubeTwoSet;
  bsg::drawableCompound* _cubeThreeSet;
  bsg::drawableCompound* _cubeFourSet;
  bsg::drawableCollection* _cubeCollection;

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
  bsg::bsgPtr<bsg::drawableObj> _cubeOne;
  bsg::bsgPtr<bsg::drawableObj> _cubeTwo;
  bsg::bsgPtr<bsg::drawableObj> _cubeThree;
  bsg::bsgPtr<bsg::drawableObj> _cubeFour;


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
              << glGetString(GL_RENDERER)  // e.g. Intel 3000 OpenGL Engine
              << " / "
              << glGetString(GL_VERSION)    // e.g. 3.2 INTEL-8.0.61
              << std::endl;

    if (glewIsSupported("GL_VERSION_2_1")) {
      std::cout << "Software check: Ready for OpenGL 2.1." << std::endl;
    } else {
      throw std::runtime_error("Software check: OpenGL 2.1 not supported.");
    }

    // This is the background color of the viewport.
    glClearColor(0.1 , 0.0, 0.4, 1.0);

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

  void _initializeScene() {

    // Create a list of lights.  If the shader you're using doesn't use
    // lighting, and the shapes don't have textures, this is irrelevant.
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

    _cubeOne = new bsg::drawableObj();
    _cubeTwo = new bsg::drawableObj();
    _cubeThree = new bsg::drawableObj();
    _cubeFour = new bsg::drawableObj();
    std::vector<glm::vec4> cubeVertices;

    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));

    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
    cubeVertices.push_back(glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));


    _cubeOne->addData(bsg::GLDATA_VERTICES, "position", cubeVertices);
    _cubeTwo->addData(bsg::GLDATA_VERTICES, "position", cubeVertices);
    _cubeThree->addData(bsg::GLDATA_VERTICES, "position", cubeVertices);
    _cubeFour->addData(bsg::GLDATA_VERTICES, "position", cubeVertices);


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
    //cubeColors.push_back(glm::vec4( 0.35, 0.51, 0.87, 1.0f));



    _cubeOne->addData(bsg::GLDATA_COLORS, "color", cubeColors);
    _cubeTwo->addData(bsg::GLDATA_COLORS, "color", cubeColors);
    _cubeThree->addData(bsg::GLDATA_COLORS, "color", cubeColors);
    _cubeFour->addData(bsg::GLDATA_COLORS, "color", cubeColors);

    _cubeOne->setDrawType(GL_TRIANGLES);
    _cubeTwo->setDrawType(GL_TRIANGLES);
    _cubeThree->setDrawType(GL_TRIANGLES);
    _cubeFour->setDrawType(GL_TRIANGLES);


    // We could put the axes and the rectangle in the same compound
    // shape, but we leave them separate so they can be moved
    // separately.
    _cubeOneSet = new bsg::drawableCompound(_shader);
    _cubeTwoSet = new bsg::drawableCompound(_shader);
    _cubeThreeSet = new bsg::drawableCompound(_shader);
    _cubeFourSet = new bsg::drawableCompound(_shader);

    _cubeOneSet->addObject(_cubeOne);
    _cubeTwoSet->addObject(_cubeTwo);
    _cubeThreeSet->addObject(_cubeThree);
    _cubeFourSet->addObject(_cubeFour);

    _cubeCollection = new bsg::drawableCollection("cubes");

    _cubeCollection->addObject(_cubeOneSet);
    _cubeCollection->addObject(_cubeTwoSet);
    _cubeCollection->addObject(_cubeThreeSet);
    _cubeCollection->addObject(_cubeFourSet);

    _cubeOneSet->setPosition(0.0f,5.0f,0.0f);
    _cubeTwoSet->setPosition(-4.0f, 0.0f, 0.0f);
    _cubeThreeSet->setPosition(4.0f, 0.0f, 0.0f);
    _cubeFourSet->setPosition(0.0f, -5.0, 0.0f);


    _axesSet = new bsg::drawableCompound(_shader);
    _axesSet->addObject(_axes);

    // Now add the axes.
    _scene.addObject(_cubeCollection);
    _scene.addObject(_axesSet);

    // All the shapes are now added to the scene.
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

    _oscillator = 0.0f;

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
      _initializeScene();
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

      glm::vec3 cubePos = _cubeCollection->getPosition();
      _oscillator += _oscillationStep;
      cubePos.x = cos(_oscillator);
      cubePos.y = 1.0f - cos(_oscillator);
      _cubeCollection->setPosition(cubePos);

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
  for (int i = 0; i < argc ; i++) {
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
