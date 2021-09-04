#ifndef __BLOCKSPACE_H
#define __BLOCKSPACE_H

#include <vector>
#include "src/Linha.h"


class BlockSpace
{
private:
	std::vector < std::vector<int>> _block_sections;
	int _width;
	float _height;
	int _nsections;
	float _sectionSize;
public:
	BlockSpace();
	BlockSpace(float width, float height, int n_sections);
	~BlockSpace();

	void Init(float width, float height, int n_sections);
	void InsertLine(Linha& line, int index);

	std::vector<int> GetLinesIndexBySection(int section) const;
	void DrawSections();
	int GetNSections() const;

};

#endif // !__BLOCKSPACE_H