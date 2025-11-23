#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>   
#include <vector>     
#include "BST.h"      
#include "Record.h"
//add header files as needed

using namespace std;

// Converts a string to lowercase (used for case-insensitive searches)
static inline string toLower(string s) {
    for (char &c : s) c = (char)tolower((unsigned char)c);
    return s;
}

// ================== Index Engine ==================
// Acts like a small "database engine" that manages records and two BST indexes:
// 1) idIndex: maps student_id → record index (unique key)
// 2) lastIndex: maps lowercase(last_name) → list of record indices (non-unique key)
struct Engine {
    vector<Record> heap;                  // the main data store (simulates a heap file)
    BST<int, int> idIndex;                // index by student ID
    BST<string, vector<int>> lastIndex;   // index by last name (can have duplicates)

    // Inserts a new record and updates both indexes.
    // Returns the record ID (RID) in the heap.
    int insertRecord(const Record &recIn) {
        //TODO
        //insert Record to heap and get its id based on the back of the heap.
        heap.push_back(recIn);
        int recordID = heap.size() - 1;

        //insert record to idIndex
        idIndex.insert(recIn.id, recordID);

        //insert record to lastIndex with lowercased last name
        std::string lowerLast = toLower(recIn.last);
        lastIndex.insert(lowerLast, std::vector<int>{recordID});

        // clear comparison metric
        idIndex.resetMetrics();
        lastIndex.resetMetrics();
        //return the recordID
        return recordID;
    }

    // Deletes a record logically (marks as deleted and updates indexes)
    // Returns true if deletion succeeded.
    bool deleteById(int id) {
        if (heap.size() == 0) return false;
        
        // gets a pointer to the heap key
        // will be null pointer if not found
        int* heapID = idIndex.find(id);

        // reset comparison metric after using find function in BST
        idIndex.resetMetrics();

        // if heapId is null pointer or data has already been soft deleted then no deletion necessary
        if(!heapID || heap[*heapID].deleted == true) return false;
        else {
            // soft delete data by setting record at heapID to deleted = true
            heap[*heapID].deleted = true;
            return heap[*heapID].deleted;
        }

    }

    // Finds a record by student ID.
    // Returns a pointer to the record, or nullptr if not found.
    // Outputs the number of comparisons made in the search.
    const Record *findById(int id, int &cmpOut) {
        if (heap.size() == 0) return nullptr;
        // get the pointer for the heap key
        // will return null pointer if key is not found
        int* heapID = idIndex.find(id);

        // store comparisons then reset metrics
        cmpOut = idIndex.comparisons;
        idIndex.resetMetrics();

        // if heapId is null pointer or data has been soft deleted then data was not found
        // else returns a pointer to the record found
        if(!heapID || heap[*heapID].deleted == true) return nullptr;
        else{
            Record* rec = &heap[*heapID];
            return rec;
        }
        }

    // Returns all records with ID in the range [lo, hi].
    // Also reports the number of key comparisons performed.
    vector<const Record *> rangeById(int lo, int hi, int &cmpOut) {
        // Create new vector that holds pointers of records in the heap
        // return empty vector if heap is empty
        vector<const Record*> recInRange;
        if(heap.size() == 0) return recInRange;

        // Checks idIndex BST to find keys that fall within the range.
        // If they are found the id is used in lambda function to get the address of the record
        idIndex.rangeApply(lo, hi,[&](const int &k, int &rid){
            recInRange.push_back(&heap[rid]);
        } );
        
        // store comparisons then reset metrics
        cmpOut = idIndex.comparisons;
        idIndex.resetMetrics();

        return recInRange;
    }

    // Returns all records whose last name begins with a given prefix.
    // Case-insensitive using lowercase comparison.
    vector<const Record *> prefixByLast(const string &prefix, int &cmpOut) {
        //TODO
        // vector<const Record*> recInPrefix;
        // if(heap.size() == 0) return recInPrefix;

        // std::string lowPrefix = toLower(prefix);
       
    }
};

#endif
