#include <vector>
#include <iostream>

#ifndef _OPTICALPATH_H_
#define _OPTICALPATH_H_

//一区間の投影光路
class OptVec
{
public:
	//値を設定する、directionとpositionには三次元の情報を入れる
	void set(std::vector<float> direction, std::vector<float> position, float material_index)
	{
		//向かう法線方向をセット
		dir = direction;

		//正規化したのちに、進む方向をセット
		float sum = 0;
		for (int i = 0; i < position.size(); i++){
			sum += position[i] * position[i];
		}
		sum = sqrt(sum);

		if (sum == 0){
			std::cout << "divide 0" << std::endl;
		}
		for (int i = 0; i < position.size(); i++){
			pos.push_back(position[i] / sum);
		}

		//現在の屈折率をセット
		index = material_index;
	}

	// m_numの値を取得する
	auto getDir() -> std::vector<float>
	{
		return dir;
	}

	auto getPos() -> std::vector<float>
	{
		return pos;
	}

	auto getIndex() -> float
	{
		return index;
	}

private:
	std::vector<float> dir;
	std::vector<float> pos;
	float index;
};


//1本の投影光路
class OptPath
{
public: 
	//光路追加
	auto set(OptVec ov) -> void
	{
		op.push_back(ov);
	}

	// n番目のOptVecを取得
	auto getOptVec(int num) -> OptVec
	{
		if (num >= op.size() || num < 0){
			std::cout << num << "_index over (optvectr)" << std::endl;
		}
		return op[num];
	}

	// OptVecのDirectionを取得する
	auto getOptVecDir(int num) -> std::vector<float>
	{
		if (num >= op.size() || num < 0){
			std::cout << num << "_index over (direction)" << std::endl;
		}
		return op[num].getDir();
	}

	// OptVecのpositionを取得
	auto getOptVecPos(int num) -> std::vector<float>
	{
		if (num >= op.size() || num < 0){
			std::cout << num << "_index over (position)" << std::endl;
		}
		return op[num].getPos();
	}

	// OptVecのmaterial indexを取得
	auto getOptVecIndex(int num) -> float
	{
		if (num >= op.size() || num < 0){
			std::cout << num << "_index over" << std::endl;
		}
		return op[num].getIndex();
	}

private:
	std::vector<OptVec> op;
};

#endif //_OPTICALPATH_H_

