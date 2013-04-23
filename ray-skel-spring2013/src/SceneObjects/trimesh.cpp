#include <cmath>
#include <float.h>
#include "trimesh.h"

using namespace std;

Trimesh::~Trimesh()
{
	for( Materials::iterator i = materials.begin(); i != materials.end(); ++i )
		delete *i;
}

// must add vertices, normals, and materials IN ORDER
void Trimesh::addVertex( const Vec3d &v )
{
    vertices.push_back( v );
}

void Trimesh::addMaterial( Material *m )
{
    materials.push_back( m );
}

void Trimesh::addNormal( const Vec3d &n )
{
    normals.push_back( n );
}

// Returns false if the vertices a,b,c don't all exist
bool Trimesh::addFace( int a, int b, int c )
{
    int vcnt = vertices.size();

    if( a >= vcnt || b >= vcnt || c >= vcnt )
        return false;

    TrimeshFace *newFace = new TrimeshFace( scene, new Material(*this->material), this, a, b, c );
    newFace->setTransform(this->transform);
    faces.push_back( newFace );
    scene->add(newFace);
    return true;
}

char *
Trimesh::doubleCheck()
// Check to make sure that if we have per-vertex materials or normals
// they are the right number.
{
    if( !materials.empty() && materials.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of materials.";
    if( !normals.empty() && normals.size() != vertices.size() )
        return "Bad Trimesh: Wrong number of normals.";

    return 0;
}

// Calculates and returns the normal of the triangle too.
bool TrimeshFace::intersectLocal( const ray& r, isect& i ) const
{
    // YOUR CODE HERE:
    // Add triangle intersection code here.
    
		Vec3d rP = r.getPosition();
		Vec3d rD = r.getDirection();
		Vec3d T[3]; // triangle points
		
	//	for (int j = 0; j < 3; j++)
	//		T[j] = vertices[j];
		
		
		Vec3d Tn; // surface normal
		double d; // extra var d
		
		Tn[0] = (T[1][1]-T[0][1])*(T[2][2]-T[0][2])-(T[2][1]-T[0][1])*(T[1][2]-T[0][2]);
		Tn[1] = (T[1][2]-T[0][2])*(T[2][0]-T[0][0])-(T[2][2]-T[0][2])*(T[1][0]-T[0][0]);
		Tn[2] = (T[1][0]-T[0][0])*(T[2][1]-T[0][1])-(T[2][0]-T[0][0])*(T[1][1]-T[0][1]);
		d     = (Tn[0]*T[0][0]+Tn[1]*T[0][1]+Tn[2]*T[0][2]);
		
		i.setN(Tn);

		// find V from ray
		double V[3];

		for (int j = 0; j < 3; j++)
			V[j] = rD[j];

		// intersect ray with plane
		// t = -(P0 • N + d) / (V • N)
		//double t = -(dot(rP, Tn) + d) / dot(V, Tn);
		double t = -(rP[0]*Tn[0]+rP[1]*Tn[1]+rP[2]*Tn[2]+d)/
								(V[0]*Tn[0]+V[1]*Tn[1]+V[2]*Tn[2]);
		i.t = t;
		
		Vec2d coords;
		
		// P = P0 + tV
		for (int j = 0; j < 3; j++)
			coords[j] = rP[j] + t*V[j];

		// make sure its inside the triangle
		double V1[3], V2[3], N1[3], d1; // for sides for triangle

		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				V1[k] = T[j][k] - rP[k];
				if (j > 1) {
					V2[k] = T[0][k] - rP[k];
				} else {
					V2[k] = T[j+1][k] - rP[k];
				}
			}
			
			// calculate cross product
			N1[0] =  V1[1]*V2[2]-V1[2]*V2[1];
			N1[1] = -V1[0]*V2[2]+V1[2]*V2[0];
			N1[2] =  V1[0]*V2[1]-V1[1]*V2[0];
			
			double mag = sqrt(N1[0]*N1[0]+N1[1]*N1[1]+N1[2]*N1[2]);
			for (int j = 0; j < 3; j++)
				N1[j] = N1[j]/mag;
			//d1 = dot(-rP, N1);
			d1 = -rP[0]*N1[0]-rP[1]*N1[1]-rP[2]*N1[2];
	
			//if ((dot(coords, N1) + d1) < 0)
			if ((coords[0]*N1[0]+coords[1]*N1[1]+coords[2]*N1[2]+d1) < 0)
				return false;
		}
		
		i.setUVCoordinates(coords);
		
    return false; 
}

void
Trimesh::generateNormals()
// Once you've loaded all the verts and faces, we can generate per
// vertex normals by averaging the normals of the neighboring faces.
{
    int cnt = vertices.size();
    normals.resize( cnt );
    int *numFaces = new int[ cnt ]; // the number of faces assoc. with each vertex
    memset( numFaces, 0, sizeof(int)*cnt );
    
    for( Faces::iterator fi = faces.begin(); fi != faces.end(); ++fi )
    {
        Vec3d a = vertices[(**fi)[0]];
        Vec3d b = vertices[(**fi)[1]];
        Vec3d c = vertices[(**fi)[2]];
        
        Vec3d faceNormal = ((b-a) ^ (c-a));
		faceNormal.normalize();
        
        for( int i = 0; i < 3; ++i )
        {
            normals[(**fi)[i]] += faceNormal;
            ++numFaces[(**fi)[i]];
        }
    }

    for( int i = 0; i < cnt; ++i )
    {
        if( numFaces[i] )
            normals[i]  /= numFaces[i];
    }

    delete [] numFaces;
}

