#include "MyMesh.h"
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_fRadius * 2.0f, a_v3Color);
	// -------------------------------

	std::vector<vector3> vertices;
	GLfloat degree = 0;
	GLfloat change = static_cast<GLfloat>(PI * 2.0 / static_cast<GLfloat>(a_nSubdivisions));
	float h = a_fHeight / 2;

	vector3 tipVector = vector3(0.0f, 0.0f, h);
	vector3 centerVector = vector3(0.0f, 0.0f, h * -1.0f);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 newVertex = vector3(cos(degree) * a_fRadius, sin(degree) * a_fRadius, h * -1.0f);
		degree += change;
		vertices.push_back(newVertex);
	}

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(centerVector, vertices[(i + 1) % a_nSubdivisions], vertices[i]);
		AddTri(vertices[i], vertices[(i + 1) % a_nSubdivisions], tipVector);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_fRadius * 2.0f, a_v3Color);
	// -------------------------------

	std::vector<vector3> verticesUpper;
	std::vector<vector3> verticesLower;
	GLfloat degree = 0;
	GLfloat change = static_cast<GLfloat>(PI * 2.0 / static_cast<GLfloat>(a_nSubdivisions));
	float h = a_fHeight / 2;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 newVecUpper = vector3(cos(degree) * a_fRadius, sin(degree) * a_fRadius, h);
		vector3 newVecLower = vector3(cos(degree) * a_fRadius, sin(degree) * a_fRadius, h * -1.0f);
		degree += change;
		verticesUpper.push_back(newVecUpper);
		verticesLower.push_back(newVecLower);
	}

	vector3 upperCenter = vector3(0.0f, 0.0f, h);
	vector3 lowerCenter = vector3(0.0f, 0.0f, h * -1.0f);

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(upperCenter, verticesUpper[i], verticesUpper[(i + 1) % a_nSubdivisions]);
		AddTri(verticesLower[i], lowerCenter, verticesLower[(i + 1) % a_nSubdivisions]);
		AddQuad(verticesLower[i], verticesLower[(i + 1) % a_nSubdivisions], verticesUpper[i], verticesUpper[(i + 1) % a_nSubdivisions]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	std::vector<vector3> outerVerticesUpper;
	std::vector<vector3> outerVerticesLower;
	std::vector<vector3> innerVerticesUpper;
	std::vector<vector3> innerVerticesLower;
	GLfloat degree = 0;
	GLfloat change = static_cast<GLfloat>(PI * 2.0 / static_cast<GLfloat>(a_nSubdivisions));
	float h = a_fHeight / 2;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 newVecLower = vector3(cos(degree) * a_fOuterRadius, sin(degree) * a_fOuterRadius, h * -1.0f);
		vector3 newVecUpper = vector3(cos(degree) * a_fOuterRadius, sin(degree) * a_fOuterRadius, h);
		degree += change;
		outerVerticesLower.push_back(newVecLower);
		outerVerticesUpper.push_back(newVecUpper);
	}

	degree = 0;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		vector3 newVecLower = vector3(cos(degree) * a_fInnerRadius, sin(degree) * a_fInnerRadius, h * -1.0f);
		vector3 newVecUpper = vector3(cos(degree) * a_fInnerRadius, sin(degree) * a_fInnerRadius, h);
		degree += change;
		innerVerticesLower.push_back(newVecLower);
		innerVerticesUpper.push_back(newVecUpper);
	}

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddQuad(innerVerticesUpper[(i + 1) % a_nSubdivisions], innerVerticesUpper[i], outerVerticesUpper[(i + 1) % a_nSubdivisions], outerVerticesUpper[i]);
		AddQuad(innerVerticesLower[i], innerVerticesLower[(i + 1) % a_nSubdivisions], outerVerticesLower[i], outerVerticesLower[(i + 1) % a_nSubdivisions]);
		AddQuad(outerVerticesLower[i], outerVerticesLower[(i + 1) % a_nSubdivisions], outerVerticesUpper[i], outerVerticesUpper[(i + 1) % a_nSubdivisions]);
		AddQuad(innerVerticesLower[(i + 1) % a_nSubdivisions], innerVerticesLower[i], innerVerticesUpper[(i + 1) % a_nSubdivisions], innerVerticesUpper[i]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	std::vector<vector3> vertex;
	GLfloat degree = 0;
	GLfloat change = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(a_nSubdivisionsA));
	float radius = (a_fOuterRadius + a_fInnerRadius) / 2;
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		vector3 temp = vector3(cos(degree) * a_fInnerRadius, sin(degree) * a_fInnerRadius, 0.0f);
		degree += change;
		vertex.push_back(temp);
	}

	GLfloat lamda = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(a_nSubdivisionsB));
	std::vector<vector3> totalVertices;
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		matrix4 m4transform = IDENTITY_M4;
		m4transform = glm::rotate(m4transform, lamda * i, vector3(0.0f, 1.0f, 0.0f));
		m4transform = glm::translate(m4transform, vector3(a_fOuterRadius, 0.0f, 0.0f));
		std::vector<vector3> vertexCopy;
		vertexCopy = vertex;

		for (int j = 0; j < a_nSubdivisionsB; j++)
		{
			vertexCopy[j] = m4transform * vector4(vertexCopy[j], 1.0f);
			totalVertices.push_back(vertexCopy[j]);
		}
	}

	int totalSize = totalVertices.size();
	int divisions = a_nSubdivisionsB;
	for (int i = 0; i < totalSize; i++)
	{
		AddQuad(totalVertices[i],
				totalVertices[(i + divisions) % totalSize],
				totalVertices[(i + 1) % totalSize],
				totalVertices[(i + divisions + 1) % totalSize]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	//GenerateCube(a_fRadius * 2.0f, a_v3Color);
	// -------------------------------

	std::vector<vector3> vertices;
	int sDivisions = a_nSubdivisions;
	int cDivisions = 6;
	GLfloat sDelta = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(sDivisions));
	GLfloat cDelta = static_cast<GLfloat>(2.0 * PI / static_cast<GLfloat>(cDivisions));

	for (int i = 0; i < sDivisions; i++)
	{
		matrix4 m4Transform = IDENTITY_M4;
		m4Transform = glm::rotate(m4Transform, sDelta * i, vector3(0.0f, 1.0f, 0.0f));

		for (int j = 0; j < cDivisions; j++)
		{
			vector3 temp = vector3(cos(cDelta * j) * a_fRadius, sin(cDelta * j) * a_fRadius, 0.0f);
			temp = m4Transform * vector4(temp, 1.0f);
			vertices.push_back(temp);
		}
	}
	
	int tVertices = vertices.size();
	for (int i = 0; i < tVertices; i++)
	{
		AddQuad(vertices[i],
			vertices[(i + cDivisions*2) % tVertices],
			vertices[(i + 1) % tVertices],
			vertices[(i + cDivisions*2 + 1) % tVertices]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}