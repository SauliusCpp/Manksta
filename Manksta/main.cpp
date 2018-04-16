#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> exerciseTypes;

struct Exercise
{
	const int GetType() { return _type; }
	int count;

	Exercise(const int& type)
	{
		_type = type;
		count = 0;
	}

	Exercise(const int& type, const int& count)
	{
		_type = type;
		this->count = count;
	}

	Exercise() = default;

	//irasyti/issaugoti mankstas
	static void WriteExercises(const char* path, Exercise* exercises, const int& count)
	{
		std::ofstream writer(path);

		for (int i = 0; i < count; i++)
		{
			int type = exercises[i].GetType();

			writer << exerciseTypes[type] << exercises[i].count << '\n';
		}

		writer.close();
	}

	//nuskaityti mankstas
	static Exercise* ReadExercises(const char* path, int& arraySize)
	{
		std::ifstream reader(path);
		if (!reader) throw std::runtime_error(std::string("Nepavyko atidaryti ") + std::string(path));

		reader >> arraySize;

		Exercise* exercises = new Exercise[arraySize];

		for (int i = 0; i < arraySize; i++)
		{
			char* typeCharStr = new char[20];
			int count;

			//praleisti \n
			reader.get();

			reader.read(typeCharStr, 20);
			reader >> count;

			std::string type(typeCharStr, 20);

			//uzregistruoti naujo tipo mankstas
			bool typeExists = false;
			int typeIndex = 0;

			for (; typeIndex < exerciseTypes.size(); typeIndex++)
			{
				if (exerciseTypes[typeIndex] == type)
				{
					typeExists = true;
					break;
				}
			}

			if (!typeExists) exerciseTypes.push_back(type);

			exercises[i] = Exercise(typeIndex, count);
		}

		reader.close();

		return exercises;
	}

	//sutraukti mankstas
	static Exercise* MergeExercises(Exercise* exercices, const int& count)
	{
		Exercise* merged = new Exercise[exerciseTypes.size()];

		for (int i = 0; i < exerciseTypes.size(); i++) merged[i] = Exercise(i, 0);
		for (int i = 0; i < count; i++)
		{
			int type = exercices[i].GetType();
			merged[type].count += exercices[i].count;
		}
		return merged;
	}

private:
	int _type;
};

int main()
{
	int size;
	Exercise* exercises = Exercise::ReadExercises("data\\U2.txt", size);
	Exercise* merged = Exercise::MergeExercises(exercises, size);
	delete[] exercises;

	//surikiuoti mankstas
	std::sort(&merged[0], &merged[exerciseTypes.size()], [](const Exercise& left, const Exercise& right) { return left.count > right.count; });

	Exercise::WriteExercises("data\\U2rez.txt.", merged, exerciseTypes.size());

	delete[] merged;
}