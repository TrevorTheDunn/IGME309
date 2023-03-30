#include "MyRigidBody.h"
using namespace BTX;
//Accessors
bool MyRigidBody::GetVisible(void) { return m_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColor(void) { return m_v3Color; }
void MyRigidBody::SetColor(vector3 a_v3Color) { m_v3Color = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
vector3 makeGlobal(matrix4 m4ToWorld, vector3 input) { return m4ToWorld * vector4(input, 1.0f); }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix) 
{ 
	if (m_m4ToWorld == a_m4ModelMatrix)
		return;

	m_m4ToWorld = a_m4ModelMatrix; 
	//Generate all 8 corners of the ARBB
	std::vector<vector3> cornerList;
	cornerList.push_back(vector3(m_v3MinL.x, m_v3MinL.y, m_v3MinL.z)); //000
	cornerList.push_back(vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z)); //001
	cornerList.push_back(vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z)); //010
	cornerList.push_back(vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z)); //011

	cornerList.push_back(vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z)); //100
	cornerList.push_back(vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z)); //101
	cornerList.push_back(vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z)); //110
	cornerList.push_back(vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MaxL.z)); //111
	//Globalize them
	for (uint i = 0; i < 8; i++)
	{
		cornerList[i] = makeGlobal(m_m4ToWorld, cornerList[i]);
	}
	//make a new box around them
	MyRigidBody oTemp(cornerList);
	m_v3MinG = oTemp.m_v3MinG;
	m_v3MaxG = oTemp.m_v3MaxG;
}
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = ModelManager::GetInstance();
	m_bVisible = true;

	m_fRadius = 0.0f;

	m_v3Color = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& other)
{
	std::swap(m_pMeshMngr , other.m_pMeshMngr);
	std::swap(m_bVisible , other.m_bVisible);

	std::swap(m_fRadius, other.m_fRadius);

	std::swap(m_v3Color , other.m_v3Color);

	std::swap(m_v3Center , other.m_v3Center);
	std::swap(m_v3MinL , other.m_v3MinL);
	std::swap(m_v3MaxL , other.m_v3MaxL);

	std::swap(m_v3MinG , other.m_v3MinG);
	std::swap(m_v3MaxG , other.m_v3MaxG);

	std::swap(m_v3HalfWidth , other.m_v3HalfWidth);

	std::swap(m_m4ToWorld , other.m_m4ToWorld);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	uint uCount = a_pointList.size();

	if (uCount < 1)
		return;

	//Finding the center by average [WRONG]
	m_v3Center = a_pointList[0];
	for (uint i = 1; i < uCount; i++)
	{
		m_v3Center += a_pointList[i];
	}
	m_v3Center /= static_cast<float>(uCount);

	m_v3Center = a_pointList[0];
	for (uint i = 1; i < uCount; i++)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	m_fRadius = 0.0f;
	for (uint i = 0; i < uCount; i++)
	{
		float fDistance = glm::distance(m_v3Center, a_pointList[i]);
		if (m_fRadius < fDistance)
			m_fRadius = fDistance;
	}
	m_fRadius = glm::distance(m_v3Center, m_v3MaxL); //Bouding Sphere of bounding box
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f; //this'll be important for HW4
}
MyRigidBody::MyRigidBody(MyRigidBody const& other)
{
	m_pMeshMngr = m_pMeshMngr;
	m_bVisible = m_bVisible;

	m_fRadius = m_fRadius;

	m_v3Color = m_v3Color;

	m_v3Center = m_v3Center;
	m_v3MinL = m_v3MinL;
	m_v3MaxL = m_v3MaxL;

	m_v3MinG = m_v3MinG;
	m_v3MaxG = m_v3MaxG;

	m_v3HalfWidth = m_v3HalfWidth;

	m_m4ToWorld = m_m4ToWorld;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyRigidBody temp(other);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody(){Release();};

//--- Non Standard Singleton Methods
vector3 MyRigidBody::GlobalizeVector(vector3 input)
{
	input = glm::rotate(quaternion(), input);
	return m_m4ToWorld * vector4(input, 1.0f);
}

void MyRigidBody::AddToRenderList(void)
{
	if (!m_bVisible)
		return;
	//for the sphere
	matrix4 m4Transform = glm::translate(vector3(m_v3Center));
	m4Transform = m4Transform * glm::scale(vector3(m_fRadius));
	//m_pMeshMngr->AddWireSphereToRenderList(m4Transform, m_v3Color);

	//for the box
	m4Transform = m_m4ToWorld * glm::translate(vector3(m_v3Center));
	m4Transform = m4Transform * glm::scale(m_v3HalfWidth * 2.0f); //this'll be important for HW4
	m_pMeshMngr->AddWireCubeToRenderList(m4Transform, m_v3Color);

	vector3 v3ARBBSize = m_v3MaxG - m_v3MinG;
	m_pMeshMngr->AddWireCubeToRenderList(
		glm::translate(makeGlobal(m_m4ToWorld, m_v3Center)) * glm::scale(v3ARBBSize), C_YELLOW);
}
bool MyRigidBody::IsColliding(MyRigidBody* const other)
{
	bool bColliding = true;

	//if the sum of the radii is > distance of their centers
	vector3 v3ThisCenterG = this->GlobalizeVector(this->m_v3Center);
	vector3 v3OtherCenterG = this->GlobalizeVector(other->m_v3Center);
	float fDistance = glm::distance(v3ThisCenterG, v3OtherCenterG);
	float fRadiiSum = this->m_fRadius + other->m_fRadius; //don't need to globalize radii unless you were scaling
	bColliding = fRadiiSum > fDistance;

	this->m_v3MinG = this->GlobalizeVector(this->m_v3MinL);
	this->m_v3MaxG = this->GlobalizeVector(this->m_v3MaxL);

	other->m_v3MinG = this->GlobalizeVector(other->m_v3MinL);
	other->m_v3MaxG = this->GlobalizeVector(other->m_v3MaxL);

	bColliding = true;

	if (m_v3MinG.x > other->m_v3MaxG.x)
		bColliding = false;
	if (m_v3MaxG.x < other->m_v3MinG.x)
		bColliding = false;

	if (m_v3MinG.y > other->m_v3MaxG.y)
		bColliding = false;
	if (m_v3MaxG.y < other->m_v3MinG.y)
		bColliding = false;

	if (m_v3MinG.z > other->m_v3MaxG.z)
		bColliding = false;
	if (m_v3MaxG.z < other->m_v3MinG.z)
		bColliding = false;

	if (bColliding)
	{
		this->m_v3Color = vector3(1.0f, 0.0f, 0.0f);
		other->m_v3Color = C_RED;
	}

	return bColliding;
}