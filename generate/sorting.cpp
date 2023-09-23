#include <string>
#include <queue>
#include <map>

#define ul unsigned long

using namespace std;

typedef struct IInterval{
        double lo;
        double hi;
} Interval;

int fileIndex = 0;

void SortIntervalFile(string source, string dest);

void CopyData(double dest[3], double source[3]) {
    for (int i = 0; i < 3; i++) dest[i] = source[i];
}

bool ReadData(FILE* f, double dest[3]) {
    size_t s = fread(dest, sizeof(double), 3, f);
    if (s == 0) return false;
    if (s == 3) return true;
    printf("ReadData needs to read 3 * double data. Seems like we couldn't.\n");
    exit(0);
}

int main(int argc, char** argv)
{
	if(argc!=3)
	{
		printf("usage: %s <source> <destination>\n", argv[0]);
		return 0;
	}

	string source = argv[1];
	string dest = argv[2];
	SortIntervalFile(source, dest);
	return 0;
}

string GetNewFileName(string tfileName) {
    fileIndex++;
    return tfileName + "_temp" + to_string(fileIndex);
}

void SaveIntervalsToAFile(map<double, Interval> intervals,
                          string newFileName) {
    
    map<double, Interval>::iterator it;
    FILE* tf = fopen(newFileName.c_str(), "w");
    for (it = intervals.begin(); it != intervals.end(); it++) {
        fwrite(&it->first, sizeof(double), 1, tf);
        fwrite(&it->second.lo, sizeof(double), 1, tf);
        fwrite(&it->second.hi, sizeof(double), 1, tf);
    }
    fclose(tf);
}

void MergeFiles(string s1, string s2, string d) {
    FILE* f1 = fopen(s1.c_str(), "r");
    FILE* f2 = fopen(s2.c_str(), "r");
    FILE* fd = fopen(d.c_str(), "w");
    
    int f1Read = 0, f2Read = 0;
    double toAdd[3], f1data[3], f2data[3];
    bool f1More, f2More;
    f1More = ReadData(f1, f1data);
    f2More = ReadData(f2, f2data);
    
    if (f1data[0] < f2data[0]) CopyData(toAdd, f1data);
    else CopyData(toAdd, f2data);
    unsigned counter = 0;
    
    while(true) {
        counter++;
        
        if (f1More && f1data[0] == toAdd[0]) {
            if (f1data[1] > toAdd[1]) toAdd[1] = f1data[1];
            if (f1data[2] < toAdd[2]) toAdd[2] = f1data[2];
            f1More = ReadData(f1, f1data);
            f1Read++;
            continue;
        }
        if (f2More && f2data[0] == toAdd[0]) {
            if (f2data[1] > toAdd[1]) toAdd[1] = f2data[1];
            if (f2data[2] < toAdd[2]) toAdd[2] = f2data[2];
            f2More = ReadData(f2, f2data);
            f2Read++;
            continue;
        }
        fwrite(toAdd, sizeof(double), 3, fd);
        if (!f1More && !f2More) break;
        
        if (f1More && !f2More) {
            CopyData(toAdd, f1data);
            f1More = ReadData(f1, f1data);
            f1Read++;
        } else if (!f1More && f2More) {
            CopyData(toAdd, f2data);
            f2More = ReadData(f2, f2data);
            f2Read++;
        } else if (f1data[0] < f2data[0]) {
            CopyData(toAdd, f1data);
            f1More = ReadData(f1, f1data);
            f1Read++;
        } else {
            CopyData(toAdd, f2data);
            f2More = ReadData(f2, f2data);
            f2Read++;
        }
    }
    
    fclose(f1);
    fclose(f2);
    fclose(fd);
} 

void SortIntervalFile(string source, string dest) {
    map<double, Interval> intervals;
    map<double, Interval>::iterator it;
    queue<string> tempFiles;
    
    FILE* f = fopen(source.c_str(), "r");
    if (!f) {
        printf("Could not open file\n");
    }
    double data[3];

    unsigned long long int counter = 0;
    while (fread(data, sizeof(double), 3, f) == 3) {
        if (data[1] <= -1.0e300 && data[2] >= 1.0e300) {
            
        } else {
            it = intervals.find(data[0]);
            if (it != intervals.end()) {
                if (data[1] > it->second.lo) it->second.lo = data[1];
                if (data[2] < it->second.hi) it->second.hi = data[2];
            } else {
                Interval temp;
                temp.lo = data[1];
                temp.hi = data[2];
                intervals[data[0]] = temp;
            }
        }
        
        counter++;
        if (counter % 1000000llu == 0llu) {
            fflush(stdout);
        }
        if (intervals.size() == 40000000) {
            string newFileName = GetNewFileName(source);
            SaveIntervalsToAFile(intervals, newFileName);
            tempFiles.push(newFileName);
            intervals.clear();
        }
    }

    fclose(f);

    if (intervals.size() > 0) {
        string newFileName = GetNewFileName(source);
        SaveIntervalsToAFile(intervals, newFileName);
        tempFiles.push(newFileName);
        intervals.clear();
    }
    
    while (tempFiles.size() > 1) {
        string tempFile1 = tempFiles.front();
        tempFiles.pop();
        string tempFile2 = tempFiles.front();
        tempFiles.pop();
        string newFileName = GetNewFileName(source);
        MergeFiles(tempFile1, tempFile2, newFileName);
        remove(tempFile1.c_str());
        remove(tempFile2.c_str());
        
        tempFiles.push(newFileName);
    }

    string tempFile1 = tempFiles.front();
    tempFiles.pop();
    rename(tempFile1.c_str(), dest.c_str());
}    
