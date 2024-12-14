#include "GPUMesh.hh"

GPUMesh::GPUMesh()
{
	numPoints = NUMPOINTS;
	points = new vec4[numPoints];
	normals= new vec4[numPoints];
    texCoords= new vec2[numPoints];
    radi_max = 1.0f;
	make();
}

GPUMesh::GPUMesh(const QString &fileName): Mesh(fileName)
{
    numPoints = NUMPOINTS;
    points = new vec4[numPoints];
    normals= new vec4[numPoints];
    texCoords= new vec2[numPoints];

    Capsa3D capsa = calculCapsa3D();

    radi_max = max(capsa.a, max(capsa.h, capsa.p)) / 2.0f;

    // textura per defecte al llegir de obj.
    setDefaultTexture();

    make();
}

GPUMesh::GPUMesh(const int npoints, const QString &fileName): Mesh(fileName)
{
    numPoints = npoints;
    points = new vec4[numPoints];
    normals= new vec4[numPoints];
    texCoords= new vec2[numPoints];

    Capsa3D capsa = calculCapsa3D();

    radi_max = max(capsa.a, max(capsa.h, capsa.p)) / 2.0f;

    // textura per defecte al llegir de obj.
    setDefaultTexture();

    make();
}

void GPUMesh::setDefaultTexture() {
    
    texture = make_shared<QOpenGLTexture>(QImage("://resources/textures/F16s.bmp").mirrored());
    initTexture();
}

void GPUMesh::read(const QJsonObject &json) {
    numPoints = NUMPOINTS;
    points = new vec4[numPoints];
    normals= new vec4[numPoints];
    texCoords= new vec2[numPoints];
    Mesh::read(json);

    // copiem els atributs del material que ja teniem (llegit a 
    // Object::read() despres de cridar Mesh::read()) per tal de 
    // que pugui ser correctament passat a la GPU

    // llegim el path de la textura i la carreguem
    if(json.contains("texture") && json["texture"].isString()) {
        QString texturePath = json["texture"].toString();
        texture = make_shared<QOpenGLTexture>(QImage(texturePath).mirrored());
    }
    else {
        // posem textura per defecte en cas que no n'hi hagi
        texture = make_shared<QOpenGLTexture>(QImage("://resources/textures/F16s.bmp").mirrored());
    }

    initTexture();

    
    auto gpumaterial = make_shared<GPUMaterial>();
    gpumaterial->Ka = material->Ka;
    gpumaterial->Kd = material->Kd;
    for (int i=0; i<NUM_TOON; i++) {
        gpumaterial->KdToon[i] = material->KdToon[i];
    }
    gpumaterial->Ks = material->Ks;
    gpumaterial->shininess = material->shininess;
    gpumaterial->opacity = material->opacity;

    material = gpumaterial;

    make();
}

/**
 * @brief GPUMesh::~GPUMesh()
 */
GPUMesh::~GPUMesh() {

    if (points!= nullptr) delete points;
    if (normals!= nullptr) delete normals;
    if (texCoords != nullptr) delete texCoords;
}

/**
 * @brief GPUMesh::toGPU
 * @param pr
 */
void GPUMesh::toGPU(shared_ptr<QGLShaderProgram> pr) {
    // TO  DO: A modificar a la fase 1 de la practica 2

    qDebug() << "Obj to GPU.....";

    program = pr;


    // activa textura i pas a GPU
    texture->bind(0);
    program->setUniformValue("texMap", 0);

    // S'envia l'uniform amb el radi
    radi_a_enviar = radi_max;
    radi_a_enviar = program->uniformLocation("radiMax");
    glUniform1f(radi_a_enviar, radi_max);

    // Creació d'un vertex array object
    glGenVertexArrays( 1, &vao );

    // Creacio i inicialitzacio d'un vertex buffer object (VBO)
    glGenBuffers( 1, &buffer );

    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal passar les normals a la GPU

    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*Index + sizeof(vec4)*Index + sizeof(vec2)*Index, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*Index, points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*Index, sizeof(vec4)*Index, normals );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*Index + sizeof(vec4)*Index, sizeof(vec2)*Index, texCoords);

    // set up vertex arrays
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(vec4)*Index));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vec4)*Index + sizeof(vec4)*Index));
    glEnableVertexAttribArray(2);

    glEnable( GL_DEPTH_TEST );
    glEnable(GL_TEXTURE_2D);

    if(dynamic_pointer_cast<GPUMaterial>(material)){
        dynamic_pointer_cast<GPUMaterial>(material)->toGPU(pr);
    }
    
}

void GPUMesh::SquaretoGPU(shared_ptr<QGLShaderProgram> pr) {

    qDebug() << "Obj to GPU.....";

    program = pr;
    // Creació d'un vertex array object

    glGenVertexArrays( 1, &vao );

    // Creacio i inicialitzacio d'un vertex buffer object (VBO)
    glGenBuffers( 1, &buffer );

    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object

    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal passar les normals a la GPU

    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*Index + sizeof(vec4)*Index, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*Index, points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*Index, sizeof(vec4)*Index, normals);

    // set up vertex arrays
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);
}



/**
 * Pintat en la GPU.
 * @brief GPUMesh::draw
 */
void GPUMesh::draw(){

    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object.
    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal activar també les normals  a la GPU

    glBindVertexArray( vao );
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    dynamic_pointer_cast<GPUMaterial>(material)->draw();

    glActiveTexture(GL_TEXTURE0);
    texture->bind(0);
    program->setUniformValue("texMap", 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    

}

/**
 * @brief GPUMesh::make
 */
void GPUMesh::make(){
    Index = 0;
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]];
            normals[Index] = normalsVertexs[cares[i].idxNormals[j]];
            //texCoords[Index] = textVertexs[cares[i].idxTextures[j]];
            Index++;
        }
	}

    // per tractar possibles mesh sense vt definides
    int k = 0;
    if(textVertexs.size() > 0) {
        for(unsigned int i=0; i<cares.size(); i++){
            for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
                texCoords[k] = textVertexs[cares[i].idxTextures[j]];
                k++;
            }
        }
    }
}


/**
 * @brief GPUMesh::initTexture
 */
void GPUMesh::initTexture()
 {
    // TO DO: A implementar a la fase 1 de la practica 2
    // Cal inicialitzar la textura de l'objecte: veure l'exemple del CubGPUTextura
    qDebug() << "Initializing textures...";

    // Carregar la textura
    glActiveTexture(GL_TEXTURE0);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    texture->bind(0);
 }

void GPUMesh::setTexture(shared_ptr<QOpenGLTexture> t){
   texture = t;
}

Capsa3D GPUMesh::calculCapsa3D()
{
    vec3    pmin, pmax;
    int     i;
    Capsa3D capsa;

    pmin.x = points[0].x;
    pmin.y = points[0].y;
    pmin.z = points[0].z;
    pmax = pmin;
	for(i=1; i<Index; i++) {
        if(points[i].x <pmin[0])
            pmin[0] = points[i].x;
        if(points[i].y <pmin[1])
            pmin[1] = points[i].y;
        if(points[i].z <pmin[2])
            pmin[2] = points[i].z;

        if(points[i].x >pmax[0])
            pmax[0] = points[i].x;
        if(points[i].y >pmax[1])
            pmax[1] = points[i].y;
        if(points[i].z >pmax[2])
            pmax[2] = points[i].z;
    }
    capsa.pmin = pmin;
    capsa.a = pmax[0]-pmin[0];
    capsa.h = pmax[1]-pmin[1];
    capsa.p = pmax[2]-pmin[2];
    return capsa;
}
