#include <iostream>
#include <unordered_map>
#include <list>

class Node{
private:
  friend class LRUCache;
  Node* next;
  Node* previous;
  int data;
  int key;
};

class LRUCache{
  public:
    LRUCache(int size): capacity(size){ 
      head = NULL;
      end = NULL;
    }
    ~LRUCache();
    int getSize(){
      return hashMap.size();
    }
    void add(Node* node);
    void remove(Node* node);
    void moveFirst(Node*);
    void removeLast();
    int get(int key);
    void set(int key, int value);

  private:
    int capacity;
    Node* head;
    Node* end;
    std::unordered_map<int, Node*> hashMap;
};

LRUCache::~LRUCache(){
  std::unordered_map<int, Node*>::const_iterator itr = this->hashMap.begin();
  while(itr != this->hashMap.end()){
    delete itr->second;
    ++itr;
  }

}

void LRUCache::add(Node* node){
  node->next = NULL;
  node->previous = NULL;

  if(this->head == NULL){
    this->head = node;
    this->end = node;
    return;
  }

  this->head->previous = node;
  node->next = this->head;
  this->head = node;
}

void LRUCache::remove(Node* node){
  if(this->head == NULL || node == NULL){
    return;
  }
  
  if(this->head == this->end && this->head == node){
    this->head = NULL;
    this->end = NULL;
    return;
  }

  if(this->head == node){
    this->head->next->previous = NULL;
    this->head = this->head->next;
    return;
  }
  
  if(this->end == node){ 
    this->end->previous->next = NULL;
    this->end = this->end->previous;
    return;
  }
  
  node->previous->next = node->next;
  node->next->previous = node->previous;
}

void LRUCache::moveFirst(Node* node){
  this->remove(node);
  this->add(node);
}

void LRUCache::removeLast(){
  this->remove(this->end); 
}

int LRUCache::get(int key){
  std::unordered_map<int, Node*>::const_iterator itr = this->hashMap.find(key);
  if(itr != this->hashMap.end()){
    Node* node = itr->second;
    this->moveFirst(node);
    return node->data;
  }

  return -1; 
}

void LRUCache::set(int key, int value){
  std::unordered_map<int, Node*>::const_iterator itr = this->hashMap.find(key);
  if(itr != this->hashMap.end()){
    Node* node = itr->second;
    node->data = value;
    this->moveFirst(node);
    return;
  }

  if(this->hashMap.size() >= this->capacity ){
    int id = this->end->key;
    Node *node = this->hashMap.find(id)->second;
    this->hashMap.erase(id);
    this->removeLast();
    delete node;
  }

  Node* node = new Node();
  node->data = value;
  node->key = key;
  this->add(node);
  this->hashMap.insert(std::make_pair(key, node));
  
}

int main(){
  LRUCache *lruCache = new LRUCache(10);
  lruCache->set(1, 2);
  lruCache->set(2, 3);
  lruCache->set(3, 4);
  lruCache->set(4, 5);
  lruCache->set(5, 6);
  lruCache->set(5, 7);
  lruCache->set(6, 6);
  lruCache->set(7, 6);
  lruCache->set(8, 6);
  lruCache->set(9, 6);
  std::cout << lruCache->get(2) << std::endl;
  lruCache->set(10, 6);
  lruCache->set(11, 6);
  lruCache->set(12, 6);
  std::cout << lruCache->get(3) << std::endl;
  delete lruCache;
}
