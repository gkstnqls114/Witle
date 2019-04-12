#pragma once

class TerrainObjectID
{
	std::string m_ID{ "" };

	TerrainObjectID(const TerrainObjectID& other);
	TerrainObjectID& operator =(const TerrainObjectID& other);
public:
	TerrainObjectID();
	virtual ~TerrainObjectID();
	 
	void InitMesh();
	void InitLight();
	void InitTransform();
	void InitCamera();
	void InitFrustum();
	void InitSkinnedMesh();

	bool isMesh() const;
	bool isLight() const;
	bool isTransform() const;
	bool isCamera() const;
	bool isFrustum() const;
	bool isSkinnedMesh() const;

	friend std::ostream& operator<<(std::ostream& os, const TerrainObjectID& TerrainObjectID);
};