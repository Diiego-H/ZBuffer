cmake_minimum_required(VERSION 3.15)

project(P2-ZBToy)

# Afegir els fitxers font
set(SOURCES
    Controller.cpp
    DataService/Output.cpp
    DataService/SetUp.cpp
    GPUConnections/GPUCamera.cpp
    GPUConnections/GPUDirectionalLight.cpp
    GPUConnections/GPULight.cpp
    GPUConnections/GPULightFactory.cpp
    GPUConnections/GPUMaterial.cpp
    GPUConnections/GPUMesh.cpp
    GPUConnections/GPUObjectFactory.cpp
    GPUConnections/GPUPointLight.cpp
    GPUConnections/GPUScene.cpp
    GPUConnections/GPUSceneFactory.cpp
    GPUConnections/GPUSceneFactoryVirtual.cpp
    GPUConnections/GPUSetUp.cpp
    GPUConnections/GPUSpotLight.cpp
    Main.cpp
    TemporalVW/Animation.cpp
    VW/Lights/DirectionalLight.cpp
    VW/Lights/Light.cpp
    VW/Lights/LightFactory.cpp
    VW/Lights/PointLight.cpp
    VW/Lights/SpotLight.cpp
    VW/Materials/Lambertian.cpp
    VW/Materials/Material.cpp
    VW/Materials/MaterialFactory.cpp
    VW/Materials/Texture.cpp
    VW/Objects/Face.cpp
    VW/Objects/Mesh.cpp
    VW/Objects/Object.cpp
    VW/Objects/ObjectFactory.cpp
    Scene.cpp
    Controller.cpp
    VW/TG/TG.cpp
    VW/TG/TranslateTG.cpp
    Render/ColorShading.cpp
    Render/ShadingFactory.cpp
    ViewGL/GLBuilder.cpp
    ViewGL/GLMainWindow.cpp
    ViewGL/GLShader.cpp
    ViewGL/GLWidget.cpp
    library/Common.cpp
)

# Add your headers
set(HEADERS
    Controller.hh
    DataService/Output.hh
    DataService/SetUp.hh
    DataService/Serializable.hh
    GPUConnections/GPUCamera.hh
    GPUConnections/GPUConnectable.hh
    GPUConnections/GPUDirectionalLight.hh
    GPUConnections/GPULight.hh
    GPUConnections/GPULightFactory.hh
    GPUConnections/GPUMaterial.hh
    GPUConnections/GPUMesh.hh
    GPUConnections/GPUObjectFactory.hh
    GPUConnections/GPUPointLight.hh
    GPUConnections/GPUScene.hh
    GPUConnections/GPUSceneFactory.hh
    GPUConnections/GPUSceneFactoryVirtual.hh
    GPUConnections/GPUSetUp.hh
    GPUConnections/GPUSpotLight.hh
    TemporalVW/Animation.hh
    Rays/HitRecord.hh
    Rays/Hittable.hh
    Rays/Ray.hh
    VW/Lights/DirectionalLight.hh
    VW/Lights/Light.hh
    VW/Lights/LightFactory.hh
    VW/Lights/PointLight.hh
    VW/Lights/SpotLight.hh
    VW/Materials/Lambertian.hh
    VW/Materials/Material.hh
    VW/Materials/MaterialFactory.hh
    VW/Materials/Texture.hh
    VW/Objects/Face.hh
    VW/Objects/Mesh.hh
    VW/Objects/Object.hh
    VW/Objects/ObjectFactory.hh
    VW/TG/TG.hh
    VW/TG/TranslateTG.hh
    Render/ColorShading.hh
    Render/ShadingFactory.hh
    Render/ShadingStrategy.hh
    ViewGL/GLBuilder.hh
    ViewGL/GLMainWindow.hh
    ViewGL/GLShader.hh
    ViewGL/GLWidget.hh
    library/Common.h
    library/Singleton.hh
    library/mat.h
    library/vec.h
)

# Add your forms
set(FORMS
ViewGL/about.ui
ViewGL/mainwindow.ui
)

# Add your resources
set(RESOURCES
    resources.qrc
)



# Configuració de Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Configuració de C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_policy(SET CMP0100 NEW)
# Make this a GUI application on Windows
if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
endif()

######################
# TODO: Set your QT5 installation path here (check following options)
######################
# Aules UB
# set(QT5_INSTALLATION_PATH "/opt/Qt/5.9.1/gcc_64")
# Ubuntu 22.04
#set(QT5_INSTALLATION_PATH "/opt/Qt5.12.12/5.12.12/gcc_64")
# set(QT5_INSTALLATION_PATH "/Users/anna/Qt15/5.15.2/clang_64/lib/cmake/Qt5")
################

set(CMAKE_PREFIX_PATH ${QT5_INSTALLATION_PATH} ${CMAKE_PREFIX_PATH})
# Ruta al ejecutable moc
set(CMAKE_AUTOMOC_MOC_EXECUTABLE ${QT5_INSTALLATION_PATH}"/bin")
# Ruta al ejecutable uic
set(CMAKE_AUTOUIC_SEARCH_PATHS ${QT5_INSTALLATION_PATH}"/bin")



# Troba els mòduls de Qt necessaris
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL )

find_package(OpenGL REQUIRED)

# Check if Qt5 is found
if(Qt5_FOUND)
    message(STATUS "Qt5 found")

    # Print out available Qt5 modules
    message(STATUS "Available Qt5 modules: ${Qt5_MODULE_NAMES}")
else()
    message(STATUS "Qt5 not found")
endif() 


include_directories(
    ${CMAKE_SOURCE_DIR}
    )


# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_VERBOSE_MAKEFILE ON)

# specify resource file names here, required for QML
# Add your resource files
qt5_add_resources(qrc ${RESOURCES})


# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS} ${qrc})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_PATH})

# Define GL_SILENCE_DEPRECATION
target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)

# Link Qt5 modules
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets ${LIBS} ${OPENGL_LIBRARIES})

