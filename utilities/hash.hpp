#ifndef __HASH__
#define __HASH__

#include <iostream>
#include <vector>
#include <string>

int hFunc(const std::vector<float> &, int, int);
int ID(const std::vector<float> &, int, int);
int gFunc(const std::vector<float> &, int, int);

template <typename T>
class Node {
public:
	const std::string key;
	T value;
	Node* next;
	
	inline Node(const std::string& key, T value): key(key), value(value), next(nullptr) {}
};

template <typename T>
class HashTable{
public:
	/*
	* @throws: "const std::string" if out of memory.
	*/
	inline HashTable() : SIZE(509) {
		table = new Node<T>*[SIZE];
		if (!table) // out of heap
			throw "Unable to create hashtable with size = " + std::to_string(SIZE) + ". Out of heap memory.";
		for (int i = 0; i < SIZE; i++)
			table[i] = nullptr;
	}

	/*
	* @throws: "const std::string" if out of memory.
	*/
	inline HashTable(unsigned int tableSize) : SIZE(tableSize) {
		table = new Node<T>*[SIZE];
		if (!table) // out of heap
			throw "Unable to create hashtable with size = " + std::to_string(SIZE) + ". Out of heap memory.";
		for (int i = 0; i < SIZE; i++)
			table[i] = nullptr;
	}

	inline ~HashTable() {
		eraseAll();
		delete table;
	}

	/*
	* If the hashtable already contains the key, the value will NOT be replaced.
	* @return: true if key-value pair was stored successfully. false if 'key' exists in the hashtable.
	*/
	inline bool add(const std::string& key, T value) {
		if (contains(key))
			return false;
		auto new_node = new Node<T>(key, value);
		if (!new_node) // out of heap
			throw "Unable to insert element with key: '" + key + "' in hashtable. Out of heap memory.";
		auto index = hash(key);
		new_node->next = table[index];
		table[index] = new_node;
		containedItems++;
		return true;
	}

	/*
	* If the hashtable already contains the key, the value will be replaced.
	*/
	inline void addOrModify(const std::string& key, T value) {
		if (!contains(key))
			add(key, value);
		else {
			auto node = table[hash(key)];
			while (node) {
				if (key == node->key) {
					node->value = value;
					return;
				}
				node = node->next;
			}
		}
	}

	/*
	* @return: true only if key is contained.
	*/
	inline bool contains(const std::string& key) const {
		auto node = table[hash(key)];
		while (node) {
			if (key == node->key)
				return true;
			node = node->next;
		}
		return false;
	}

	/*
	* @return: The value assosciated with the key.
	* @throws: "const std::string" if key is not contained in the hashtable.
	*/
	inline T get(const std::string& key) const {
		auto node = table[hash(key)];
		while (node) {
			if (key == node->key)
				return node->value;
			node = node->next;
		}
		throw "Hashtable doesn't contain an item with key '" + key + "'.";
	}

	/*
	* @return: true only if item was found and removed.
	*/
	inline bool erase(const std::string& key) {
		auto index = hash(key);
		auto node = table[index];
		auto prevNode = node;

		if (node == nullptr)
			return false;

		if (key == node->key) {
			table[index] = node->next;
			delete node;
			containedItems--;
			return true;
		}

		node = node->next;
		while (node) {
			if (key == node->key) {
				prevNode->next = node->next;
				delete node;
				containedItems--;
				return true;
			}
			prevNode = node;
			node = node->next;
		}
	}

	/*
	* Deletes all elements of the hashtable.
	*/
	inline void eraseAll() {
		Node<T> *node, *prevNode;
		for (int i = 0; (!isEmpty()) && (i < SIZE); i++) {
			if (table[i]) {
				node = table[i];
				while (node) {
					prevNode = node;
					node = node->next;
					delete prevNode;
					containedItems--;
				}
				table[i] = nullptr;
			}
		}
	}

	/*
	* @return: true only if the hashtable is empty.
	*/
	inline bool isEmpty() {
		return containedItems == 0;
	}

	/*
	* @return: the number of items contained in the hashtable.
	*/
	inline unsigned int get_current_size() {
		return containedItems;
	}

	/*
	* DEBUG ONLY
	*/
	inline void PRINT() {
		Node<T>* node, * prevNode;
		for (int i = 0; (!isEmpty()) && (i < SIZE); i++) {
			if (table[i]) {
				node = table[i];
				while (node) {
					prevNode = node;
					node = node->next;
					std::cerr << prevNode->key << " : " << prevNode->value << std::endl;
				}
				table[i] = nullptr;
			}
		}
	}

private:
	const unsigned int SIZE;
	static const unsigned int HASH_MULTIPLIER = 65599;
	Node<T>** table;
	unsigned int containedItems = 0;

	inline unsigned int hash(const std::string& key) const {
		const char* c_key = key.c_str();
		unsigned int i = 0U, hashValue = 0U;
		while (c_key[i])
			hashValue = hashValue * HASH_MULTIPLIER + c_key[i++];
		return (hashValue % SIZE);
	}
};

#endif
