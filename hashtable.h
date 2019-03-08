#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include "linked_list.h"
#include <cassert>

template <class T>
struct HashTableIterator {
  unsigned int bucket;
  ListNode<T> *node;

  T item() const {
    return node->item;
  }
};

/*
  A hash table for storing items. It is assumed the type T of the item
  being stored hash a hash method, eg. you can call item.hash(), which
  returns an unsigned integer.

  Also assumes the != operator is implemented for the item being stored,
  so we could check if two items are different same.

  If you just want store integers int for the key, wrap it up in a struct
  with a .hash() method and an != operator.
*/

template <typename T>
class HashTable {
public:
  // creates an empty hash table of the given size.
  HashTable(unsigned int tableSize) {
    assert(tableSize > 0);

    table = new LinkedList<T>[tableSize];
    assert(table != NULL);

    this->tableSize = tableSize;
    numItems = 0;
  }

  ~HashTable() {
    // will call the destructor for each list in the table
    delete[] table;
  }

  // Check if the item already appears in the table.
  bool contains(const T& item) const;

  // Insert the item, do nothing if it is already in the table.
  // Returns true iff the insertion was successful (i.e. the item was not there).
  bool insert(const T& item);

  // Removes the item after checking, via assert, that the item was in the table.
  void remove(const T& item);

  HashTableIterator<T> startIterator() const;
  HashTableIterator<T> nextIterator(const HashTableIterator<T>& iter) const;

  bool isEndIterator(const HashTableIterator<T>& iter) const;

  unsigned int size() const { return numItems; }

private:
  LinkedList<T> *table;
  unsigned int numItems;
  unsigned int tableSize;

  // Computes which hash table bucket the item maps to.
  unsigned int getBucket(const T& item) const {
    return item.hash() % tableSize;
  }
};

template <typename T>
bool HashTable<T>::contains(const T& item) const {
  // just get the item's bucket and use the lists find feature
  unsigned int hashVal = getBucket(item);
  return (table[hashVal].find(item) != NULL);
}

template <typename T>
bool HashTable<T>::insert(const T& item) {
  // get the item's bucket
  unsigned int bucket = getBucket(item);

  // If the item is not in this bucket, insert it.
  // Use insertFront, it always takes O(1) time.
  if (table[bucket].find(item) == NULL) {
    table[bucket].insertFront(item);
    numItems++;
    return true;
  }
  else {
    // the item was already in the table
    return false;
  }
}

template <typename T>
void HashTable<T>::remove(const T& item) {
  unsigned int bucket = getBucket(item);

  ListNode<T> *node = table[bucket].find(item);
  assert(node != NULL); // make sure the item was in the table

  table[bucket].removeNode(node);
  numItems--;
}

template <typename T>
HashTableIterator<T> HashTable<T>::startIterator() const {
  HashTableIterator<T> iter;
  if (numItems == 0) {
    // indicates this is after all items
    iter.bucket = tableSize;
    iter.node = NULL;
  }
  else {
    for (iter.bucket = 0; table[iter.bucket].size() == 0; iter.bucket++);
    iter.node = table[iter.bucket].getFirst();
  }
  return iter;
}

template <typename T>
HashTableIterator<T> HashTable<T>::nextIterator(const HashTableIterator<T>& iter) const {
  HashTableIterator<T> next = iter;

  next.node = next.node->next;

  // this bucket is done, find the next nonempty bucket
  if (next.node == NULL) {
    next.bucket++;
    while (next.bucket < tableSize && table[next.bucket].size() == 0) {
      next.bucket++;
    }
    if (next.bucket < tableSize) {
      next.node = table[next.bucket].getFirst();
    }
    else {
      next.node = NULL;
    }
  }
  return next;
}

template <typename T>
bool HashTable<T>::isEndIterator(const HashTableIterator<T>& iter) const {
  return (iter.bucket == tableSize);
}

#endif
