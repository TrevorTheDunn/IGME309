#include "Octant.h"
using namespace BTX;
//  Octant
uint Octant::m_uOctantCount = 0;
uint Octant::m_uMaxLevel = 3;
uint Octant::m_uIdealEntityCount = 5;
Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	/*
	* This constructor is meant to be used ONLY on the root node, there is already a working constructor
	* that will take a size and a center to create a new space
	*/
	Init();//Init the default values
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	//create a rigid body that encloses all the objects in this octant, it necessary you will need
	//to subdivide the octant based on how many objects are in it already an how many you IDEALLY
	//want in it, remember each subdivision will create 8 children for this octant but not all children
	//of those children will have children of their own

	//The following is a made-up size, you need to make sure it is measuring all the object boxes in the world
	std::vector<vector3> lMinMax;

	//loops through all entities
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		//Gets the entity at i
		Entity* pEntity = nullptr;
		pEntity = m_pEntityMngr->GetEntity(i);
		
		//gets the rigid body of the entity (if the entity exists)
		RigidBody* pRB = nullptr;
		if (pEntity)
			pRB = pEntity->GetRigidBody();

		//if the rigid body exists, gets the min and max of the RB and pushes them onto the minMax vector
		if (pRB)
		{
			vector3 min = pRB->GetMinGlobal();
			vector3 max = pRB->GetMaxGlobal();

			lMinMax.push_back(min);
			lMinMax.push_back(max);
		}
	}
	
	//constructs rigid body from minMax vector to get below variables
	RigidBody pRigidBody = RigidBody(lMinMax);

	//The following will set up the values of the octant, make sure the are right, the rigid body at start
	//is NOT fine, it has made-up values
	m_fSize = pRigidBody.GetHalfWidth().x * 2.0f;
	m_v3Center = pRigidBody.GetCenterLocal();
	m_v3Min = m_v3Center - pRigidBody.GetHalfWidth();
	m_v3Max = m_v3Center + pRigidBody.GetHalfWidth();

	//clears minMax vector
	lMinMax.clear();

	m_uOctantCount++; //When we add an octant we increment the count
	ConstructTree(m_uMaxLevel); //Construct the children
}

bool Octant::IsColliding(uint a_uRBIndex)
{
	//Get how many objects there are in the world
	uint totalEnt = m_pEntityMngr->GetEntityCount();

	//If the index given is larger than the number of elements in the bounding object there is no collision
	if (a_uRBIndex > totalEnt) return false;

	//As the Octree will never rotate or scale this collision is as easy as an Axis Alligned Bounding Box
	//Get all vectors in global space (the octant ones are already in Global)

	//Gets the entity at the specified index
	Entity* oEnt = nullptr;
	oEnt = m_pEntityMngr->GetEntity(a_uRBIndex);

	//Gets the rigid body of the entity (if it exists)
	RigidBody* oRB = nullptr;
	if (oEnt) oRB = oEnt->GetRigidBody();

	//if the rigid body exists, checks if it is colliding with the current octant
	if (oRB)
	{
		vector3 oMin = oRB->GetMinGlobal();
		vector3 oMax = oRB->GetMaxGlobal();

		vector3 tMax = m_v3Max;
		vector3 tMin = m_v3Min;

		if (tMax.x < oMin.x) return false;
		if (tMin.x > oMax.x) return false;

		if (tMax.y < oMin.y) return false;
		if (tMin.y > oMax.y) return false;

		if (tMax.z < oMin.z) return false;
		if (tMin.z > oMax.z) return false;

		return true;
	}

	return false; //returns false if the rigid body does not exist
}
void Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Display the specified octant

	//Base case of recursive call - when the current octant has the specified index, displays the octant and returns
	if (m_uID == a_nIndex)
	{
		m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
			glm::scale(vector3(m_fSize)), a_v3Color);

		return;
	}

	//recursive portion - loops through children and calls their display method and passes in the index that was passed down
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_nIndex);
	}
}
void Octant::Display(vector3 a_v3Color)
{
	//this is meant to be a recursive method, in starter code will only display the root
	//even if other objects are created

	//recursive portion - loops through childrern and calls display, passing in the color that was passed into this method
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color);
	}

	//once all children have displayed, displays the current octant
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::Subdivide(void)
{
	//If this node has reach the maximum depth return without changes
	if (m_uLevel >= m_uMaxLevel)
		return;

	//If this node has been already subdivided return without changes
	if (m_uChildren != 0)
		return;

	m_uChildren = 8; //sets amount of children to 8

	//Subdivide the space and allocate 8 children
	float childSize = this->m_fSize / 2.0f;
	float offset = childSize / 2.0f;

	//Child 1 - Top Right Front
	vector3 childCenter = vector3(m_v3Center.x + offset, m_v3Center.y + offset, m_v3Center.z + offset);
	m_pChild[0] = new Octant(childCenter, childSize);
	
	//Child 2 - Top Right Back
	childCenter.z -= childSize;
	m_pChild[1] = new Octant(childCenter, childSize);

	//Child 3 - Top Left Back
	childCenter.x -= childSize;
	m_pChild[2] = new Octant(childCenter, childSize);

	//Child 4 - Top Left Front
	childCenter.z += childSize;
	m_pChild[3] = new Octant(childCenter, childSize);

	//Child 5 - Bottom Right Front
	childCenter.y -= childSize;
	childCenter.x += childSize;
	m_pChild[4] = new Octant(childCenter, childSize);

	//Child 6 - Bottom Right Back
	childCenter.z -= childSize;
	m_pChild[5] = new Octant(childCenter, childSize);

	//Child 7 - Bottom Left Back
	childCenter.x -= childSize;
	m_pChild[6] = new Octant(childCenter, childSize);

	//Child 8 - Bottom Left Front
	childCenter.z += childSize;
	m_pChild[7] = new Octant(childCenter, childSize);

	//loops through all children assigning root, parent, and the level
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;

		//checks if the child contains more than the ideal entity count (that's why it is plus 1). if it does, child subdivides
		if (m_pChild[i]->ContainsAtLeast(m_uIdealEntityCount + 1))
		{
			m_pChild[i]->Subdivide();
		}
	}
}
bool Octant::ContainsAtLeast(uint a_nEntities)
{
	//You need to check how many entity objects live within this octant

	uint count = 0; //count of entities

	//loops through all entities - if an entity is colliding increments count, if amount of entities equals to the passed amount, returns true
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		if (IsColliding(i)) count++;

		if (count >= a_nEntities) return true;
	}

	return false; //returns false if amount of entities in current octant is less than the passed amount
}
void Octant::AssignIDtoEntity(void)
{
	//Recursive method
	//Have to traverse the tree and make sure to tell the entity manager
	//what octant (space) each object is at

	//recursive portion - calls this method for all children of current octant
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDtoEntity();
	}

	//if the octant doesn't have any children - checks for colliding entities, adds their IDs from EntityManager to list and assigns them to current octant
	if (m_uChildren == 0)
	{
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			if (IsColliding(i))
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
// You can assume the following is fine and does not need changes, you may add onto it but the code is fine as is
// in the proposed solution.
void Octant::Init(void)
{
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = nullptr;
	}
}
void Octant::Swap(Octant& other)
{
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
void Octant::Release(void)
{
	//this is a special kind of release, it will only happen for the root
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}
void Octant::ConstructTree(uint a_nMaxLevel)
{
	//If this method is tried to be applied to something else
	//other than the root, don't.
	if (m_uLevel != 0)
		return;

	m_uMaxLevel = a_nMaxLevel; //Make sure you mark which is the maximum level you are willing to reach
	m_uOctantCount = 1;// We will always start with one octant

	//If this was initialized before make sure its empty
	m_EntityList.clear();//Make sure the list of entities inside this octant is empty
	KillBranches();
	m_lChild.clear();

	//If we have more entities than those that we ideally want in this octant we subdivide it
	if (ContainsAtLeast(m_uIdealEntityCount))
	{
		Subdivide();
	}
	AssignIDtoEntity();//Add octant ID to Entities
	ConstructList();//construct the list of objects
}
//The big 3
Octant::Octant(vector3 a_v3Center, float a_fSize)
{
	//Will create the octant object based on the center and size but will not construct children
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	m_uOctantCount++;
}
Octant::Octant(Octant const& other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;

	m_pRoot, other.m_pRoot;
	m_lChild, other.m_lChild;

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}
Octant& Octant::operator=(Octant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Octant temp(other);
		Swap(temp);
	}
	return *this;
}
Octant::~Octant() { Release(); };
//Accessors
float Octant::GetSize(void) { return m_fSize; }
vector3 Octant::GetCenterGlobal(void) { return m_v3Center; }
vector3 Octant::GetMinGlobal(void) { return m_v3Min; }
vector3 Octant::GetMaxGlobal(void) { return m_v3Max; }
uint Octant::GetOctantCount(void) { return m_uOctantCount; }
bool Octant::IsLeaf(void) { return m_uChildren == 0; }
Octant* Octant::GetParent(void) { return m_pParent; }
//--- other methods
Octant * Octant::GetChild(uint a_nChild)
{
	//if its asking for more than the 8th children return nullptr, as we don't have one
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}
void Octant::KillBranches(void)
{
	/*This method has recursive behavior that is somewhat hard to explain line by line but
	it will traverse the whole tree until it reaches a node with no children and
	once it returns from it to its parent it will kill all of its children, recursively until
	it reaches back the node that called it.*/
	for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_uChildren = 0;
}
void Octant::DisplayLeaves(vector3 a_v3Color)
{
	/*
	* Recursive method
	* it will traverse the tree until it find leaves and once found will draw them
	*/
	uint nLeafs = m_lChild.size(); //get how many children this will have (either 0 or 8)
	for (uint nChild = 0; nChild < nLeafs; nChild++)
	{
		m_lChild[nChild]->DisplayLeaves(a_v3Color);
	}
	//Draw the cube
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::ClearEntityList(void)
{
	/*
	* Recursive method
	* It will traverse the tree until it find leaves and once found will clear its content
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ClearEntityList();
	}
	m_EntityList.clear();
}
void Octant::ConstructList(void)
{
	/*
	* Recursive method
	* It will traverse the tree adding children
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ConstructList();
	}
	//if we find a non-empty child add it to the tree
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}
