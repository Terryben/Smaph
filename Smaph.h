//Ben Terry
//Smaph Class.  A smaph is a container very similar to a map that also sorts it's elements, and does not allow duplicates.  The Insert function preserves pointers, erase and clear do not.

#ifndef SMAPH_H_
#define SMAPH_H_

#include <utility>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <climits>


template <typename key_type, typename mapped_type, class Compare = std::less <std::pair <key_type, mapped_type> > >

class Smaph {

  public:
    typedef unsigned int size_type;
    typedef std::pair<key_type, mapped_type> value_type;
  private:

	struct node {
		struct node* next;
		struct node* prev;
		key_type first;
		mapped_type second;
	};
	node *head, *tail;
 
  public:
   
   //ITERATOR FOR SMAPH
   class iterator {
	private:
		node* p;
	        friend class Smaph;
		iterator(node* ptr) : p(ptr){}
	public:
		iterator() : p(0) { }
		iterator(const iterator &it)  {
			p = it.p;
		}
		~iterator(){
			//delete p;
		}
		//Drecrement is generally allowed on .end() but that was unneeded so I didnt add that functionality.  Don't decrement .end()
		Smaph::iterator &operator--() {	// Predecrement
	   	 	assert(p);
	   		p = p->prev;
	   	 	return *this;
		}
		Smaph::iterator operator--(int) {	// postdecrement
	   	 	const iterator tmp = *this;
	  	 	--*this;
	   	 	return tmp;
		}
		Smaph::iterator &operator++() {	// Preincrement
	   	 	assert(p);
	   		p = p->next;
	   	 	return *this;
		}
		Smaph::iterator operator++(int) {	// Postincrement
	  	 	const iterator tmp = *this;
	  	 	++*this;
	  	  	return tmp;
		}
		// *iterator: Return a reference to the datum
		const node &operator*() const {
	   		assert(p);
	    		return (*p);
		}
		// iterator->: Return the address of the datum
		const node *operator->() const {
	  		  assert(p);
	  		  return p;
		}

		  Smaph::iterator &operator=(const iterator  &it) 	{//equals operator
			p = it.p;
			return *this;
		}
		iterator operator+(int num) const{//plus operator addition
			iterator temp(*this);
			for(int i = 0;i<num;i++){
				++temp;
			}
			return temp;
		}
		iterator operator-(int num) const{//subtraction operator
			iterator temp(*this);
			for(int i = 0;i<num;i++){
				--temp;
			}
			return temp;
		}
		

		bool operator==(iterator  it) const	{//comparison operator
			return (p == it.p); 
				
		}

		bool operator!=(iterator  it) const	{//not equals operator
			return !(p == it.p);
		}
   };
/////END OF ITERATOR METHODS




   Smaph(key_type a, mapped_type b) {//constructor
	head = new node;
	(*head).first = a;
	(*head).second = b;
	(*head).next = 0;
	(*head).prev = 0;
	tail = head;
     }

   Smaph(const value_type  vt) {
	head = new node; 
	(*head).first = vt.first;
	(*head).second = vt.second;
	(*head).next = 0;
	(*head).prev = 0;
	tail = head;
   }   

   Smaph() : head(0), tail(0) {
     }

   ~Smaph() {
	clear();
     }

    //Return an iterator at the start of the smaph
    Smaph::iterator begin() const {
	return iterator(head);
   }
    //Didn't need the functionality so this doesn't actually return an iterator at the end of the smaph.  Could change it to return a special end node so that it functions more like other containers if needed.
    Smaph::iterator end() const {
	return iterator(0);
   }

    Smaph::iterator find(const key_type & kt, const mapped_type & mt) const{//searches through the smaph and returns an iterator at the location of the object in question or end() if the element is not found
		node *p = head;
		while(p != tail->next){
			if(p->first == kt && p->second == mt){
				return iterator(p);	
			}
			p = p->next;
		}
		
		return end();	
	}

    Smaph::iterator find(const value_type & vt) const {//calls find
	return (*this).find(vt.first, vt.second);
   }

   size_type count(const key_type & kt, const mapped_type & mt) const{//calls find
	if(find(kt, mt) == (*this).end())
		return 0;
	return 1;
	}
   size_type count(const value_type &vt) const {//calls find
	if(find(vt) == (*this).end())
		return 0;
	return 1;
	}

   std::pair<iterator,bool> insert(const key_type & kt, const mapped_type & mt){//inserts the element using the functor
	node *p = new node;
	p->first = kt;
	p->second = mt;
	node *it = head;
	std::pair<key_type, mapped_type> myPair = std::make_pair(kt, mt);
	bool b;
	Compare comp;
	if (!head){
	    	head = p;
	    	tail = p;
		std::pair<iterator, bool> ret;
		ret = std::make_pair(iterator(p), true);
		return ret;
	}
	else{
			if(comp(myPair, std::make_pair(it->first, it->second))){//if myPair goes before the head
					p->next = head;
					p->prev = 0;
					head->prev = p;
					head = p;
					std::pair<iterator, bool> ret;
					ret = std::make_pair(iterator(head), true);		
					return ret;
			}
			while(it != 0){
				
				std::pair<key_type, mapped_type> myPair = std::make_pair(kt, mt);
				if(comp(myPair, std::make_pair(it->first, it->second))){//if myPair goes before the iterator node					
					p->next = it;
					p->prev = it->prev;
					it->prev->next = p;
					it->prev = p;
					std::pair<iterator, bool> ret;
					ret = std::make_pair(iterator(--it), true);		
					return ret;
				}
				else if(!comp(std::make_pair(it->first, it->second), myPair) && !comp(myPair, std::make_pair(it->first, it->second))){//myPair is equal with the it node
					std::pair<iterator, bool> ret;
					ret = std::make_pair(iterator(it), false);
					return ret;//value was already there	
				}
				else{//move on to the next node
				it = it->next;

				}
			
			}//value goes at the end of the smaph
			tail->next = p;
			p->next = 0;
			p->prev = tail;
			tail = p;
			std::pair<iterator, bool> ret;
			ret = std::make_pair(iterator(tail), true);	
			return ret;
		}
	}
//insert with a value type, breaks the vt apart and calls insert
std::pair<iterator,bool> insert(const value_type & vt){
	return insert(vt.first, vt.second);
}
//insert with iterators.  Inserts all values starting with the first up until the second
   void insert(iterator it, iterator it2){
	while(it != it2){
		insert(it->first, it->second);
		++it;
	}
   }
//removes all values from the smaph
   void clear() {
	while (head != NULL && head) {
	    node *p = head->next;
	    head = NULL;
	    delete head;
	    head = p;
	}

   }

bool erase(const key_type & kt, const mapped_type & vt) {//erases the given argument from the Smaph
	iterator it = find(kt, vt);
	if(it == head && it == tail){
		node* temp = head;
		head = 0;
		tail = 0;
		delete temp;
		return true;
	}
	if(it == head){
		head = it->next;
		delete (head->prev);
		head->prev = 0;
		return true;
	}
	if(it == tail){
		tail = it->prev;
		delete tail->next;
		tail->next = 0;
		return true;

	}
	if(it == end()){
		return false;
	}
	it->prev->next = it->next;
	it->next->prev = it->prev;
	return true;
}

bool erase(const value_type & vt){//calls erase
	return erase(vt.first, vt.second);


}

void erase(iterator it){//calls erase
	erase (it->first, it->second);
}

void erase(iterator it, iterator it2){//calls erase
	while(it != it2){
		iterator temp = it;
		++it;
		erase (temp->first, temp->second);
		
	}
}
//is the smaph empty?
   bool empty() const {
	return !head;
    }
//returns the number of elements in the smaph
   size_type size() const {
	int count=0;
	if(!head){
		return 0;
	}
	for (node *p = head; p; p=p->next)
	    count++;
	return count;
    }

   size_type max_size() const {
	return UINT_MAX / sizeof(node);
	}

//DEFINING OPERATORS FOR THE SMAPH

  Smaph operator|(Smaph &sm2){//UNION
	Smaph ret;
	ret.insert((*this).begin(), (*this).end());
	ret.insert(sm2.begin(), sm2.end());
	return ret;
  }
 Smaph operator|(value_type &vt){//UNION with value type
	Smaph ret;
	ret.insert((*this).begin(), (*this).end());
	ret.insert(vt);
	return ret;
  }

 friend Smaph operator|(value_type &vt, Smaph &sm){//Union with value type not using THIS
	Smaph ret;
	ret.insert(sm.begin(), sm.end());
	ret.insert(vt);
	return ret;
  }

 Smaph operator&(Smaph &sm2){//INTERSECTION
	Smaph ret;
	iterator it = (*this).begin();
	while(it != (*this).end()){
		if(sm2.find(it->first, it->second) != sm2.end()){
			ret.insert(it->first, it->second);	
		}
		++it;

	}
	return ret;
  }
 Smaph operator&(value_type &vt){//INTERSECTION
	Smaph ret;
	
		if((*this).find(vt) != (*this).end()){
			ret.insert(vt);	
		}
	return ret;
  }

 friend Smaph operator&(value_type &vt, Smaph &sm){//INTERSECTION
	Smaph ret;
	
		if(sm.find(vt) != sm.end()){
			ret.insert(vt);	
		}
	return ret;
  }

 Smaph operator|=(value_type &vt){//UNION EQUALS
	(*this).insert(vt);
	return *this;
 }
Smaph operator|=(Smaph &sm){//UNION EQUALS
	(*this).insert(sm.begin(), sm.end());
	return *this;
 }
Smaph operator&=(Smaph &sm2){//INTERSECTION EQUALS
	Smaph ret;
	iterator it = (*this).begin();
	while(it != (*this).end()){
		if(sm2.find(it->first, it->second) != sm2.end()){
			ret.insert(it->first, it->second);	
		}
		++it;
	}
	(*this).clear();
	iterator it2 = ret.begin();
	while(it2 != ret.end()){
		(*this).insert(it2->first, it2->second);
		++it2;
	}

	return (*this);
}
Smaph operator&=(value_type &vt){//INTERSECTION EQUALS VT
	if((*this).find(vt) != (*this).end()){
		(*this).clear();
		(*this).insert(vt);
		return *this;
	}
	(*this).clear();
	return (*this);
}



};

 

#endif 
