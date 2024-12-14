#include <QVector>
#include <QVector3D>
#include "GPUConnections/GPUMaterial.hh"


#include "Mesh.hh"

Mesh::Mesh(const QString &fileName): Object()
{
    nom = fileName;

    // Posem un material per defecte
    auto mat = make_shared<GPUMaterial>(
        vec3(1.0f, 0.5f, 0.31f),
        vec3(1.0f, 0.5f, 0.31f),
        vec3(0.5f, 0.5f, 0.5f),
        16.0
    );
    setMaterial(mat);
    
    load(fileName);
}


/**
 * @brief Mesh::~Mesh()
 */
Mesh::~Mesh() {
    if (cares.size() > 0) cares.clear();
    if (vertexs.size() > 0) vertexs.clear();
}


void Mesh::makeTriangles() {
    // Practica 1: TO DO Fase 1: A implementar
}



bool Mesh::hit(Ray &raig, float tmin, float tmax) const {

    // TODO Practica 1: Tutorial 1: A implementar
    return false;

}
bool Mesh::allHits(Ray& raig, float tmin, float tmax) const {
    // TODO Practica 1: Tutorial 1: A implementar
    return false;
}


void Mesh::aplicaTG(shared_ptr<TG> t) {
    // Practica 1: TO DO: Fase 1
}

void Mesh::load (QString fileName) {
    QFile file(fileName);
    if(file.exists()) {
        if(file.open(QFile::ReadOnly | QFile::Text)) {
            while(!file.atEnd()) {
                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split(QRegularExpression("\\s+"));
                if(lineParts.count() > 0) {
                    // if it’s a comment
                    if(lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0)
                    {
                        // qDebug() << line.remove(0, 1).trimmed();
                    }

                    // si és un mtllib l'haurem de llegir
                    if(lineParts.at(0).compare("mtllib", Qt::CaseInsensitive) == 0)
                    {
                        string filename = lineParts.at(1).toStdString();

                        // ara hauríem de llegir el material
                        // convertir QString a std::string
                        string filePathStr = fileName.toStdString();

                        size_t lastSlashPos = filePathStr.find_last_of('/');
                        
                        // Construïm el nou path assumint que el mtllib esta al mateix directori
                        string newFilePathStr = filePathStr.substr(0, lastSlashPos + 1) + filename;
                        QString materialPath = QString::fromStdString(newFilePathStr);

                        loadMaterial(materialPath);
                    }


                    // if it’s a vertex position (v)
                    else if(lineParts.at(0).compare("v", Qt::CaseInsensitive) == 0)
                    {
                        vertexs.push_back(vec4(lineParts.at(1).toFloat(),
                                               lineParts.at(2).toFloat(),
                                               lineParts.at(3).toFloat(), 1.0f));
                    }

                    // if it’s a normal (vn)
                    else if(lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0)
                    {
                        normalsVertexs.push_back (vec4(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat(),
                                            lineParts.at(3).toFloat(), 0.0f));
                    }
                    // if it’s a texture (vt)
                    else if(lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0)
                    {
                        textVertexs.push_back(vec2(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat()));
                    }

                    // if it’s face data (f)
                    // there’s an assumption here that faces are all triangles
                    else if(lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0)
                    {
                        Face *face = new Face();

                        // get points from v array
                        face->idxVertices.push_back(lineParts.at(1).split("/").at(0).toInt() - 1);
                        face->idxVertices.push_back(lineParts.at(2).split("/").at(0).toInt() - 1);
                        face->idxVertices.push_back(lineParts.at(3).split("/").at(0).toInt() - 1);
                        if(textVertexs.size() > 0) // check if really there are any UV coords
                        {
                            face->idxTextures.push_back( lineParts.at(1).split("/").at(1).toInt() - 1);
                            face->idxTextures.push_back( lineParts.at(2).split("/").at(1).toInt() - 1);
                            face->idxTextures.push_back( lineParts.at(3).split("/").at(1).toInt() - 1);
                        }

                        // get normals from vn array
                        face->idxNormals.push_back( lineParts.at(1).split("/").at(2).toInt() - 1);
                        face->idxNormals.push_back( lineParts.at(2).split("/").at(2).toInt() - 1);
                        face->idxNormals.push_back( lineParts.at(3).split("/").at(2).toInt() - 1);

                        // cara->calculaNormal();

                        cares.push_back(*face);
                    }
                }
            }
            file.close();
        } else {
            qWarning("Boundary object file can not be opened.");
        }
    } else  qWarning("Boundary object file not found.");
}

void Mesh::read (const QJsonObject &json)
{
    Object::read(json);
    if (json.contains("objFileName") && json["objFileName"].isString()) {
        nom = json["objFileName"].toString();
        load(nom);
    }
}


//! [1]
void Mesh::write(QJsonObject &json) const
{
    Object::write(json);
    json["objFileName"] = nom;
}
//! [1]

void Mesh::print(int indentation) const
{
    Object::print(indentation);
    const QString indent(indentation * 2, ' ');
    QTextStream(stdout) << indent << "objFileName:\t" << nom << "\n";
}

void Mesh::loadMaterial (QString fileName) {
    vec3 ka = vec3(1.0f, 0.5f, 0.31f);
    vec3 kd = vec3(1.0f, 0.5f, 0.31f);
    vec3 ks = vec3(0.5f, 0.5f, 0.5f);
    float shininess = 0.0;          

    QFile file(fileName);
    if(file.exists()) {
        if(file.open(QFile::ReadOnly | QFile::Text)) {
            while(!file.atEnd()) {
                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split(QRegularExpression("\\s+"));
                if(lineParts.count() > 0) {
                    if(lineParts.at(0).compare("Ka", Qt::CaseInsensitive) == 0)
                    {
                        ka = vec3(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat(),
                                            lineParts.at(3).toFloat());
                    }

                    else if(lineParts.at(0).compare("Kd", Qt::CaseInsensitive) == 0)
                    {
                        kd = vec3(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat(),
                                            lineParts.at(3).toFloat());
                    }

                    else if(lineParts.at(0).compare("Ks", Qt::CaseInsensitive) == 0)
                    {
                        ks = vec3(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat(),
                                            lineParts.at(3).toFloat());
                    }

                    else if(lineParts.at(0).compare("Ns", Qt::CaseInsensitive) == 0)
                    {
                        shininess = lineParts.at(1).toFloat();
                    }                          
               }
            }
            file.close();

            auto mat = make_shared<GPUMaterial>(
            ka, kd, ks, shininess
            );
            setMaterial(mat);

        } else {
            // Si no és vàlid el fitxer mtllib llavors carreguem el següent material per defecte
            if(material == nullptr){
                auto mat = make_shared<GPUMaterial>(
                    vec3(1.0f, 0.5f, 0.31f),
                    vec3(1.0f, 0.5f, 0.31f),
                    vec3(0.5f, 0.5f, 0.5f),
                    16.0
                );
                setMaterial(mat);
            }  
            qWarning("Boundary object file can not be opened.");
        }
    } else
    {  
        // Si no és vàlid el fitxer mtllib llavors carreguem el següent material per defecte
        // Comprovem que no tinguem material ja, es prioritza si s'ha carregat des d'un json
        if(material == nullptr){
            auto mat = make_shared<GPUMaterial>(
                vec3(1.0f, 0.5f, 0.31f),
                vec3(1.0f, 0.5f, 0.31f),
                vec3(0.5f, 0.5f, 0.5f),
                16.0
            );
            setMaterial(mat);
        }
        
    }
}
            
            
            






