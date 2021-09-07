#include "BlockSpasce.h"

BlockSpace::BlockSpace()
{
	this->_width = 0;
	this->_height = 0;
	this->_nsections = 0;
	this->_sectionSize = 0;
	this->_block_sections.clear();
}

BlockSpace::BlockSpace(float width, float height, int n_sections): _width(width), _height(height), _nsections(n_sections)
{
	this->_block_sections.clear();
	this->_sectionSize = 0;
	this->Init(this->_width, this->_height, this->_nsections);
}

BlockSpace::~BlockSpace()
{
	// Do nothing
}

void BlockSpace::Init(float width, float height, int n_sections)
{
	// Split only into vertical sections
	this->_nsections = n_sections;
	this->_sectionSize = width / n_sections;
	this->_width = width;
	this->_height = height;
	this->_block_sections.clear();
	this->_block_sections.resize(static_cast<size_t>(_nsections));
	
	for (int i = 0; i < _nsections; i++)
		this->_block_sections[i].clear();
}

void BlockSpace::InsertLine(Linha& line, int index)
{	
	int o_section = static_cast<int>(line.x1 / this->_sectionSize);
	int f_section = static_cast<int>(line.x2 / this->_sectionSize);

	o_section = (o_section >= this->_nsections) ? this->_nsections - 1 : o_section;
	f_section = (f_section >= this->_nsections) ? this->_nsections - 1 : f_section;
	o_section = (o_section < 0) ? 0 : o_section;
	f_section = (f_section < 0) ? 0 : f_section;

	int dif = f_section - o_section;

	if (dif == 0)
	{
		this->_block_sections[o_section].push_back(index);
		return;
	}

	int cont = 0;

	do 
	{
		cont = o_section + dif;		
		this->_block_sections[cont].push_back(index);
		dif += (dif < 0) ? 1 : -1;

	} while (dif != 0);
	this->_block_sections[o_section].push_back(index);
}

std::vector<int> BlockSpace::GetLinesIndexBySection(int section) const
{
	return std::vector<int>();//this->_block_sections[section];
}

std::vector<std::vector<int>> BlockSpace::GetAllBlocks() const
{
	return this->_block_sections;
}

void BlockSpace::DrawSections()
{
	float x1 = 0;
	float sz = 100.f / this->_nsections;

	for (int i = 0; i < this->_nsections; i++)
	{
		x1 = sz * i;
		glBegin(GL_LINES);
			glVertex2f(x1, 0.f);
			glVertex2f(x1, 500.f);
		glEnd();
	}
}

int BlockSpace::GetNSections() const
{
	return this->_nsections;
}
