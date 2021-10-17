//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://vreng.enst.fr/
//
// Copyright (C) 1997-2008 Philippe Dax
// Telecom-ParisTech (Ecole Nationale Superieure des Telecommunications)
//
// VREng is a free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public Licence as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// VREng is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//---------------------------------------------------------------------------
//***************************************//
// Author (10/06/01) :                   //
// Ben Humphrey, Game Programmer         //
// DigiBen@GameTutorials.com             //
// Co-Web Host of www.GameTutorials.com  //
//                                       //
// Vreng port (30/03/04) :               //
// Romain Piegay                         //
// piegay@enst.fr                        //
// piegaro.free.fr/piegaropro            //
//***************************************//
#include "vreng.hpp"
#include "ase.hpp"
#include "http.hpp"	// httpOpen
#include "texture.hpp"	// open
#include "cache.hpp"	// openCache
#include "file.hpp"	// closeFile


// Below are a all the important tag defines for reading in a .Ase file.
#define OBJECT		"*GEOMOBJECT"		// An object tag for new objects
#define NUM_VERTEX	"*MESH_NUMVERTEX"	// The number of vertices tag
#define NUM_FACES	"*MESH_NUMFACES"	// The number of faces tag
#define NUM_TVERTEX	"*MESH_NUMTVERTEX"	// The number of texture coordinates
#define VERTEX		"*MESH_VERTEX"		// The list of vertices tag
#define FACE		"*MESH_FACE"		// The list of faces tag
#define NORMALS		"*MESH_NORMALS"		// The list of normals tag (If you want)
#define FACE_NORMAL	"*MESH_FACENORMAL"	// The face normal for the current index
#define NVERTEX		"*MESH_VERTEXNORMAL"	// The list of vertex normals
#define TVERTEX		"*MESH_TVERT"		// The texture coordinate index tag
#define TFACE		"*MESH_TFACE"		// The vertex index tag
#define TEXTURE		"*BITMAP"		// The file name for the object's texture map
#define UTILE		"*UVW_U_TILING"		// The U tiling ratio tag
#define VTILE		"*UVW_V_TILING"		// The V tiling ratio tag
#define UOFFSET		"*UVW_U_OFFSET"		// The U tile offset tag
#define VOFFSET		"*UVW_V_OFFSET"		// The V tile offset tag
#define MATERIAL_ID	"*MATERIAL_REF"		// The material ID tag
#define MATERIAL_COUNT	"*MATERIAL_COUNT"	// The material count tag
#define MATERIAL	"*MATERIAL"		// The material tag
#define MATERIAL_NAME	"*MATERIAL_NAME"	// The material name tag
#define MATERIAL_COLOR	"*MATERIAL_DIFFUSE"	// The material color tag



Ase::Ase(const char *_url)
 : loaded(false), currentScale(1.0), desiredScale(1.0), fp(NULL)
{
  url = new char[strlen(_url) + 1];
  strcpy(url, _url);
  Http::httpOpen(url, httpReader, this, 0);
  displaylist();
}

bool Ase::loadFromFile(FILE *f)
{
  fp = f;

  if (importModel(&ASEModel) == false) { error("ASE file not loaded"); return false; }
  if (importTextures() == false) warning("textures of ASE file not loaded");
  return loaded = true; //success
}

Ase::~Ase()
{
  for (int i=0; i < ASEModel.numObjects; i++) {
    delete[] ASEModel.pObject[i].pFaces;
    delete[] ASEModel.pObject[i].pNormals;
    delete[] ASEModel.pObject[i].pVerts;
    delete[] ASEModel.pObject[i].pTexVerts;
  }
  if (url) delete[] url;
  if (dlist > 0) glDeleteLists(dlist, 1);
}

bool Ase::importTextures()
{
  int id = 0;

  for (int i=0; i < ASEModel.numMaterials; i++) {
    if (strlen(ASEModel.pMaterials[i].strFile) > 0) {
      id = loadTexture(ASEModel.pMaterials[i].strFile);
      if (id < 0) {
        error("texture %s not loaded", ASEModel.pMaterials[i].strFile);
        return false;
      }
    }
    ASEModel.pMaterials[i].texureId = id;
    trace(DBG_VGL, "importTexture: id=%d", id);
  }
  return true;
}

void Ase::bindTexture2D(int textureId)
{
  if (textureId > 0) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
  }
}

int Ase::loadTexture(const char *imgFile)
{
  char *end = strrchr(url, '/');
  int i=0;
  char url_tex[255];

  for (char *p = url; p != end; i++, p++)
    url_tex[i] = *p;
  sprintf(&url_tex[i], "/%s", imgFile);
  trace(DBG_VGL, "Ase::loadTexture: url=%s", url_tex);

  return Texture::open(url_tex);
}

float Ase::getRadius()
{
  double max_radius = 0.0;

  for (int i=0; i < ASEModel.numObjects; i++) {
    if (ASEModel.pObject.size() <= 0) break;
    tASEObject *pObject = &ASEModel.pObject[i];
    for (int j=0; j < pObject->numFaces; j++)
      for (int whichVertex = 0; whichVertex < 3; whichVertex++) {
        int vertIndex = pObject->pFaces[j].vertIndex[whichVertex];
        double r= pObject->pVerts[vertIndex].x * pObject->pVerts[vertIndex].x +
                  pObject->pVerts[vertIndex].y * pObject->pVerts[vertIndex].y +
                  pObject->pVerts[vertIndex].z * pObject->pVerts[vertIndex].z ;
        if (r > max_radius) max_radius = r;
      }
  }
  return sqrt(max_radius);
}

void Ase::setScale(float scale)
{
  if (scale != currentScale) {
    if (! loaded)
      desiredScale = scale;
    else {
      currentScale = desiredScale=scale;
      for (int i=0; i < ASEModel.numObjects; i++) {
        if (ASEModel.pObject.size() <= 0) break;
        tASEObject *pObject = &ASEModel.pObject[i];
        for (int j=0; j < pObject->numVerts; j++) {
          pObject->pVerts[ j ].x *= scale;
          pObject->pVerts[ j ].y *= scale;
          pObject->pVerts[ j ].z *= scale;
        }
      }
    }
  }
}

const char * Ase::getUrl() const
{
  return (const char *) url;
}

void Ase::httpReader(void *aase, Http *http)
{
  Ase *ase = (Ase *) aase;
  if (! ase) return;

  FILE *f = Cache::openCache(ase->getUrl(), http);
  ase->loadFromFile(f);
}

void Ase::render()
{
  if (! loaded) return;

  glPushMatrix();
  glScalef(currentScale, currentScale, currentScale);
  glCallList(dlist);
  glPopMatrix();
}

// Draws in the display list
GLint Ase::displaylist()
{
  dlist = glGenLists(1);
  glNewList(dlist, GL_COMPILE);
  draw();
  glEndList();
  return dlist;
}

void Ase::draw()
{
  if (! loaded) return;

  if (currentScale != desiredScale)
    setScale(desiredScale);

  for (int i=0; i < ASEModel.numObjects; i++) {
    if (ASEModel.pObject.size() <= 0) break;

    tASEObject *pObject = &ASEModel.pObject[i]; // get the current object

    if (pObject->bHasTexture) {
      glColor3ub(255, 200, 200);  // the color to normal again

      bindTexture2D(ASEModel.pMaterials[pObject->materialID].texureId); // Bind the texture
    }
    else {
      glDisable(GL_TEXTURE_2D);
      glEnable(GL_COLOR);
      glEnable(GL_COLOR_MATERIAL);
      glColor3ub(255, 200, 200);
    }

    glBegin(GL_TRIANGLES);  // Begin drawing with our selected mode

    for (int j=0; j < pObject->numFaces; j++) {
      for (int whichVertex=0; whichVertex < 3; whichVertex++) {
        int vi = pObject->pFaces[j].vertIndex[whichVertex];

#if 1 //DAX
        if (vi < 0) continue;    //BUG: segfault
#endif
        glNormal3f(pObject->pNormals[vi].x, pObject->pNormals[vi].y, pObject->pNormals[vi].z);

        if (pObject->bHasTexture) {
          if (pObject->pTexVerts) {
            int coordIndex = pObject->pFaces[j].coordIndex[whichVertex];

            glTexCoord2f(pObject->pTexVerts[coordIndex].x, pObject->pTexVerts[coordIndex].y);
          }
        }
        else {
          if (ASEModel.pMaterials.size() && pObject->materialID >= 0) {
            float *pColor = ASEModel.pMaterials[pObject->materialID].fColor;

            glColor3f(pColor[0], pColor[1], pColor[2]);
          }
        }
        glVertex3f(pObject->pVerts[vi].x, pObject->pVerts[vi].y, pObject->pVerts[vi].z);
      }
    }
    glEnd();		// End the drawing
    glColor3f(0,0,0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_COLOR_MATERIAL);
  }
}

/** called to load in an .ase file by the file name */
bool Ase::importModel(tASEModel *pModel)
{
  if (!pModel || !fp) return false;

  readFile(pModel);
  computeNormals(pModel);

  File::closeFile(fp);
  return true;
}

/** reads the data for every object and it's associated material*/
void Ase::readFile(tASEModel *pModel)
{
  tASEMaterialInfo newMaterial;	// This will be used to push on a new material
  tASEObject newObject = {0};	// This will be used to push on a new object

  // This function is the head for reading in the .ase data and information.
  // We count the objects then we count the materials.
  // Then for each material and each object we read the data in.

  // This will return the number of objects stored in the .ase file
  pModel->numObjects   = getObjectCount();

  // This will return the number of materials stored in the .ase file
  pModel->numMaterials = getMaterialCount();

  // Go through all the materials and fill in their data and info
  for (int i=0; i < pModel->numMaterials; i++) {
    // Add a new material to our list of materials using the STL "vector" class
    pModel->pMaterials.push_back(newMaterial);

    // Get the material info for the current material.  We add 1 because we
    // want to start at 1 and i starts at 0.
    getTextureInfo(&(pModel->pMaterials[i]), i + 1);
  }

  // Go through all the objects and fill in their data and info
  for (int i=0; i < pModel->numObjects; i++) {
    // Add a new object to our list of objects using the STL "vector" class
    pModel->pObject.push_back(newObject);

    // Set the material ID to -1 to initialize it.  This will be changed
    // if there is a texture/material assigned to this object.
    pModel->pObject[i].materialID = -1;

    // Move the file pointer to the desired object.  We add one because our
    // object count starts at 1 and i starts at 0
    moveToObject(i + 1);

    // Find out the number of vertices, faces and texture coordinates for this object,
    // then allocate the memory needed to store that amount that needs to be read in.
    readObjectInfo(&(pModel->pObject[i]), i + 1);

    // Read the vertices, face indices and texture coordinates for this object
    readObjectData(pModel, &(pModel->pObject[i]), i + 1);
  }
}

/** returns the total object count in the .ase file */
int Ase::getObjectCount()
{
  char line[255] = {0};
  int cnt = 0;

  // finds an OBJECT tag, it increases the object count.
  rewind(fp);
  while (!feof(fp)) {
    fscanf(fp, "%s", line);
    if (!strcmp(line, OBJECT)) cnt++;
    else fgets(line, sizeof(line), fp);
  }
  return cnt;
}

/** returns the total material count for the .ase file */
int Ase::getMaterialCount()
{
  char line[255] = {0};
  int materialCount = 0;

  // finds the MATERIAL_COUNT tag, it reads the material count.
  rewind(fp);
  while (!feof(fp)) {
    fscanf(fp, "%s", line);
    if (!strcmp(line, MATERIAL_COUNT)) {
      fscanf(fp, "%d", &materialCount);
      return materialCount;
    }
    else fgets(line, sizeof(line), fp);
  }
  return 0;	// Return NO materials if we get here
}

/** reads in the information about the desired material */
void Ase::getTextureInfo(tASEMaterialInfo *pTexture, int desiredMaterial)
{
  char line[255] = {0};
  int materialCount= 0;

  // In this function we check for an MATERIAL tag, then see if it's the
  // material we want by the desiredMaterial number.  If so we start reading it in.
  rewind(fp);
  while (!feof(fp)) {
    fscanf(fp, "%s", line);
    if (!strcmp(line, MATERIAL)) {
      materialCount++;
      // Check if it's the one we want to stop at
      if (materialCount == desiredMaterial) break;
    }
    else fgets(line, sizeof(line), fp);
  }

  // Go through the rest of the file until we hit the end
  while (!feof(fp)) {
    fscanf(fp, "%s", line);

    // If we found a MATERIAL tag stop because we went to far
    if (! strcmp(line, MATERIAL)) {
      // We hit a new material, so we need to stop
      return;
    }
    // If we hit a MATERIAL_COLOR tag, we need to get the material's color
    else if (! strcmp(line, MATERIAL_COLOR)) {
      // Get the material RGB color of the object
      fscanf(fp, " %f %f %f", &(pTexture->fColor[0]),
                              &(pTexture->fColor[1]),
                              &(pTexture->fColor[2]));
    }
    // If we hit a TEXTURE tag, we need to get the texture's name
    else if (!strcmp(line, TEXTURE)) {
      // Get the file name of the texture
      getTextureName(pTexture);
    }
    // If we hit a MATERIAL_NAME tag, we need to get the material's name
    else if (! strcmp(line, MATERIAL_NAME)) {
      // Get the material name of the object
      getMaterialName(pTexture);
    }
    // If we hit a UTILE tag, we need to get the U tile ratio
    else if (! strcmp(line, UTILE)) {
      // Read the U tiling for the U coordinates of the texture
      pTexture->uTile = readFloat();
    }
    // If we hit a VTILE tag, we need to get the V tile ratio
    else if (! strcmp(line, VTILE)) {
      // Read the V tiling for the V coordinates of the texture
      pTexture->vTile = readFloat();
    }
    // Otherwise ignore the data and read past it
    else fgets(line, sizeof(line), fp);
  }
}

/** moves the filepointer in the .ase file to the desired object */
void Ase::moveToObject(int desiredObject)
{
  char line[255] = {0};
  int cnt = 0;

  // takes the number of the desired object to move to.
  // We read through the file from the beginning and increase a counter.
  // When that counter gets to the desiredObject we then stop.  Now the file
  // pointer can read the object data from that specific object

  rewind(fp);
  while (!feof(fp)) {
    fscanf(fp, "%s", line);
    if (! strcmp(line, OBJECT)) {
      cnt++;
      // Check if it's the one we want to stop at, if so stop reading
      if (cnt == desiredObject)
        return;
    }
    else fgets(line, sizeof(line), fp);
  }
}

/** reads in and returns a float from the .ase file */
float Ase::readFloat()
{
  float v = 0.0;

  fscanf(fp, " %f", &v);
  return v;
}

/** reads in the information about the object, but not the data */
void Ase::readObjectInfo(tASEObject *pObject, int desiredObject)
{
  char line[255] = {0};

  // In this function we position the file pointer to the object we want
  // to read in, then we read until we find the associated tags, then read the info.

  // Go to the desired object to read from in the file
  moveToObject(desiredObject);

  while (!feof(fp)) {
    fscanf(fp, "%s", line);
    if (! strcmp(line, NUM_VERTEX)) {
      fscanf(fp, "%d", &pObject->numVerts);
      pObject->pVerts = new Vec3[pObject->numVerts];
    }
    else if (! strcmp(line, NUM_FACES)) {
      // Read in the number of faces for this object
      fscanf(fp, "%d", &pObject->numFaces);
      pObject->pFaces = new tASEFace [pObject->numFaces];
    }
    else if (! strcmp(line, NUM_TVERTEX)) {
      // Read in the number of texture coordinates for this object
      fscanf(fp, "%d", &pObject->numTexVertex);
      pObject->pTexVerts = new Vec2[pObject->numTexVertex];
    }
    // If we hit the object tag we want to stop cause we went to far
    else if (! strcmp(line, OBJECT)) {
      // Return if we get to the next object
      return;
    }
    else {
      //We didn't find anything we want to check for so we read to the next line
      fgets(line, sizeof(line), fp);
    }
  }
}

/** reads in the filename of the texture assigned to the object */
void Ase::getTextureName(tASEMaterialInfo *pTexture)
{
  fscanf(fp, " \"%s", pTexture->strFile);
  pTexture->strFile[strlen (pTexture->strFile) - 1] = '\0';
}

/** reads in the material name of the object */
void Ase::getMaterialName(tASEMaterialInfo *pTexture)
{
  fscanf(fp, " \"%s", pTexture->strName);
  pTexture->strName[strlen (pTexture->strName)] = '\0';
}

/** calls the functions that query and read in the desired data */
void Ase::readObjectData(tASEModel *pModel, tASEObject *pObject, int desiredObject)
{
  // This will go through the file every time for each data tag and read
  // in the data for that tag. We pass in the desired object to read it from.

  // Load the material ID for this object
  getData(pModel, pObject, (char*)MATERIAL_ID, desiredObject);

  // Load the vertices for this object
  getData(pModel, pObject, (char*)VERTEX, desiredObject);

  // Load the texture coordinates for this object
  getData(pModel, pObject, (char*)TVERTEX, desiredObject);

  // Load the vertex faces list for this object
  getData(pModel, pObject, (char*)FACE, desiredObject);

  // Load the texture face list for this object
  getData(pModel, pObject, (char*)TFACE, desiredObject);

  // Load the texture for this object
  getData(pModel, pObject, (char*)TEXTURE, desiredObject);

  // Load the U tile for this object
  getData(pModel, pObject, (char*)UTILE, desiredObject);

  // Load the V tile for this object
  getData(pModel, pObject, (char*)VTILE, desiredObject);
}

/** loop for reading in the object data */
void Ase::getData(tASEModel *pModel, tASEObject *pObject, char *strDesiredData, int desiredObject)
{
  char line[255] = {0};

  // In here, we move the file pointer to the desired object we want to read in,
  // then we query all the tags that are being read in.  If we find a valid tag,
  // then we need to check if that's the tag that we want to read in.  If so,
  // then we read it in.

  // Move the file pointer to the desired object
  moveToObject(desiredObject);

  while (! feof(fp)) {
    fscanf(fp, "%s", line);

    if (! strcmp(line, OBJECT)) { return; }
    // If we hit a vertex tag
    else if (! strcmp(line, VERTEX)) {
      if (! strcmp(strDesiredData, VERTEX)) {
        readVertex(pObject);
      }
    }
    // If we hit a texture vertex
    else if (! strcmp(line, TVERTEX)) {
      if (! strcmp(strDesiredData, TVERTEX)) {
         readTextureVertex(pObject, pModel->pMaterials[pObject->materialID]);
      }
    }
    // If we hit a vertice index to a face
    else if (! strcmp(line, FACE)) {
      if (! strcmp(strDesiredData, FACE)) {
        readFace(pObject);
      }
    }
    // If we hit a texture index to a face
    else if (! strcmp(line, TFACE)) {
      if (! strcmp(strDesiredData, TFACE)) {
        readTextureFace(pObject);
      }
    }
    // If we hit the material ID to the object
    else if (! strcmp(line, MATERIAL_ID)) {
      if (! strcmp(strDesiredData, MATERIAL_ID)) {
        pObject->materialID = (int)readFloat();
        return;
      }
    }
    else fgets(line, sizeof(line), fp);
  }
}

/** reads in the vertices for the object */
void Ase::readVertex(tASEObject *pObject)
{
  int index = 0;

  fscanf(fp, "%d", &index); // read past the vertex index

  // Here we read in the vertice's (X, Y, Z). Since 3D Studio Max has Z pointing
  // up, we are going to swap Y and Z to make it normal. Otherwise we would need
  // to rotate it around the X axis by -90deg: glRotatef(-90, 1.0f, 0.0f, 0.0f);
  fscanf(fp, "%f %f %f", &pObject->pVerts[index].x,
                         &pObject->pVerts[index].z,
                         &pObject->pVerts[index].y);

  // Also in 3D studio max the Z goes out of the screen so we want to swap that.
  // We do that by negating the Z value of each vertex.
  pObject->pVerts[index].z = -pObject->pVerts[index].z;
}

/** reads in the texture coordinates */
void Ase::readTextureVertex(tASEObject *pObject, tASEMaterialInfo texture)
{
  int index = 0;

  fscanf(fp, "%d", &index); // read the index of the texture coordinate

  // Next, we read in the (U, V) texture coordinates.
  fscanf(fp, "%f %f", &(pObject->pTexVerts[index].x), &(pObject->pTexVerts[index].y));

  // What is being done here is we are multiplying a X and Y tile factor
  // to the UV coordinate.  Usually the uTile and vTile is 1, but if it depends on
  // your UVW map.  If you made the texture tile more along the object, these would change.
  pObject->pTexVerts[index].x *= texture.uTile;
  pObject->pTexVerts[index].y *= texture.vTile;

  // We know this object has a texture so let's set this true
  pObject->bHasTexture = true;
}

/** reads in the indices to the array of vertices */
void Ase::readFace(tASEObject *pObject)
{
  int index = 0;

  fscanf(fp, "%d:", &index); // read the index of this face

  // Now we read in the actual vertex indices; One index for each point in the triangle.
  // These indices will index into the vertex array pVerts[].
  fscanf(fp, "\tA:\t%d B:\t%d C:\t%d", &(pObject->pFaces[index].vertIndex[0]),
                                       &(pObject->pFaces[index].vertIndex[1]),
                                       &(pObject->pFaces[index].vertIndex[2]));
}

/** reads in the indices to the texture coordinate array */
void Ase::readTextureFace(tASEObject *pObject)
{
  int index = 0;

  fscanf(fp, "%d:", &index);

  // Now we read in the UV coordinate index for the current face.
  // This will be an index into pTexCoords[] for each point in the face.
  fscanf(fp, "%d %d %d", &pObject->pFaces[index].coordIndex[0],
                         &pObject->pFaces[index].coordIndex[1],
                         &pObject->pFaces[index].coordIndex[2]);
}

/** This function computes the normals and vertex normals of the objects */
void Ase::computeNormals(tASEModel *pModel)
{
  Vec3 v1, v2, vNormal, vPoly[3];

  if (pModel->numObjects <= 0) return;

  // Go through each of the objects to calculate their normals
  for (int index = 0; index < pModel->numObjects; index++) {
    // Get the current object
    tASEObject *pObject = &(pModel->pObject[index]);
    int i;

    // Here we allocate all the memory we need to calculate the normals
    Vec3 *pNormals     = new Vec3 [pObject->numFaces];
    Vec3 *pTempNormals = new Vec3 [pObject->numFaces];
    pObject->pNormals  = new Vec3 [pObject->numVerts];

    for (i=0; i < pObject->numFaces; i++) { // go though all of the faces
      // to cut down LARGE code, we extract the 3 points of this face
      vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
      vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
      vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

      // calculate the face normals (Get 2 vectors and find the cross product of those 2)
      v1 = Vector(vPoly[0], vPoly[2]); // Get the vector of the polygon (we just need 2 sides for the normal)
      v2 = Vector(vPoly[2], vPoly[1]); // Get a second vector of the polygon
      vNormal  = Cross(v1, v2);  // Return the cross product of the 2 vectors (normalize vector, but not a unit vector)
      pTempNormals[i] = vNormal;             // Save the un-normalized normal for the vertex normals
      vNormal  = Normalize(vNormal);         // Normalize the cross product to give us the polygons normal
      pNormals[i] = vNormal;                 // Assign the normal to the list of normals
    }

    // Now Get The Vertex Normals //
    Vec3 vSum = {0.0, 0.0, 0.0};
    Vec3 vZero = vSum;
    int shared=0;

    for (i=0; i < pObject->numVerts; i++) {	// go through all of the vertices
      for (int j=0; j < pObject->numFaces; j++) {  // go through all of the triangles
      	// check if the vertex is shared by another face
        if (pObject->pFaces[j].vertIndex[0] == i ||
            pObject->pFaces[j].vertIndex[1] == i ||
            pObject->pFaces[j].vertIndex[2] == i) {
          vSum = AddVector(vSum, pTempNormals[j]);// Add the un-normalized normal of the shared face
          shared++;		// Increase the number of shared triangles
        }
      }
      // get the normal by dividing the sum by the shared.  We negate the shared so it has the normals pointing out.
      pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));
      pObject->pNormals[i] = Normalize(pObject->pNormals[i]);
      vSum = vZero;		// Reset the sum
      shared = 0;		// Reset the shared
    }
    if (pTempNormals) delete[] pTempNormals;
    if (pNormals) delete[] pNormals;
  }
}
