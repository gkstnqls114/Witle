#pragma once

class FamilyID
{
	std::string m_ID {""};

	FamilyID(const FamilyID& other);
	FamilyID& operator =(const FamilyID& other);
public:
	FamilyID();
	virtual ~FamilyID();
	
	void InitTEST();
	bool isTEST();

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

	friend std::ostream& operator<<(std::ostream& os, const FamilyID& familyID);
};