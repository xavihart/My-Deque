#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP
 
#include "exceptions.hpp"
 
#include <cstddef>
 
namespace sjtu
{
 
template <class T>
class deque
{
private:
	 int Node_size;
  int length;
	struct Node
	{
		T **data;
		Node *prev;
		Node *next;
		int size ;
 
		Node(int s, Node *b = NULL, Node *c = NULL)
		{
 
			data = new T *[s] ;
			next = c;
			prev = b;
			size=0;
		}
		~Node() {delete []data;}
	};
 
 
	Node *head, *rear;
	
 
	void merge(Node*p){
		int len1=p->size;
		int len2=p->next->size;
		    for(int i=len1;i<=len1+len2-1;++i){
					p->data[i]=p->next->data[i-len1];
				}
Node *tmp=p->next;
p->next=tmp->next;
p->next->prev=p;
for(int i=0;i<len2;++i){
	delete tmp->data[i];
	tmp->data[i]=NULL;
}				
delete []tmp->data;
delete tmp;
 
	}
 
	void split (Node* s,int p){
      Node *tmp=s->next;
			 s->next=new Node(Node_size,s,s->next);
			 /////   s..->()->..s
	  if(tmp!=NULL)  tmp->prev==s->next;
		for(int i=0;i<s->size-p;++i){
			  s->next->data[i]=s->next->data[i+p-1];
		}
	  for(int i=p;i<s->size;++i)
		{
			delete s->data[i];
      s->data[i]=NULL;
		}
		s->size=p;
		s->next->size=s->size-p;
		
	}
 
	void selfclear()
	{
 
		Node *tmp,*p=head;
	  
		while (p != NULL)
		{
		tmp=p->next;
		for(int i=0;i<p->size;++i){
	    
			
			 delete p->data[i];
			 p->data[i]=NULL;
		}
	  delete p;
 
			p=tmp;
		}
 
		   head=rear=NULL;
	}
 
	void findposition(const size_t q,Node* &a,int &p)const{
		  size_t n=q+1;
			Node* tmp;
			 if(n>length)   {a=NULL;p=-1;return;}
			 
				 
				 
				 tmp=head;
				 
				 
	      while(n>tmp->size){
					n-=tmp->size;
					tmp=tmp->next;
				   
				}
       a=tmp;
			 p=n-1;
 
 
	}
 
public:
	class const_iterator;
	class iterator
	{
friend deque;
friend const_iterator;
 
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
 
		Node *current_node;
		int position; //////////   0,1,2......
	public:
			 iterator():current_node(NULL),position(-1){}
			 iterator(Node *s, int a){current_node=s;position=a;}
 
	   iterator&operator=(const iterator &s){
		    current_node=s.current_node;
				position=s.position;
				return *this;
	   }
	
		
		
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
          
 
            if(current_node==NULL)  throw invalid_iterator();
			if (n < 0)
				return (*this) - (-n);
			iterator tmp(*this);
		
	int counter = n;
				while (tmp.current_node->size - tmp.position <= counter)
				{
					counter -= tmp.current_node->size;
					if (tmp.current_node->next != NULL)tmp.current_node = tmp.current_node->next;
					else
					{
						counter += tmp.current_node->size;
						tmp.position += counter;
						return tmp;
					}
				}
				tmp.position += counter;
				return tmp;
 
 
		}
		iterator operator-(const int &n) const
		{
  
			if(current_node==NULL)  throw invalid_iterator();
			if (n < 0) return (*this) + (-n);
 
			int tot = 0;
			iterator tmp(*this);
		 	int c = n;
				while (tmp.position < c)
				{
					c -= (tmp.position + 1);
					if (tmp.current_node->prev != NULL)
					{
						tmp.current_node = tmp.current_node->prev;
						tmp.position = tmp.current_node->size - 1;
					}
					else
					{
				
						c += (tmp.position + 1);
						tmp.position -= c;
						return tmp;
				
					}
				}
				tmp.position -= c;
				return tmp;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
           if(current_node==NULL)  throw invalid_iterator();
			Node *p = current_node, *q = rhs.current_node;
			int tot = 0;
			if (p == q)
				return position - (rhs.position);
				tot+=(current_node->size-position-1);
				p=p->next;
			while (p != NULL)
			{
 
				if (p == q)
				{
					tot += rhs.position+1 ;
					break;
				}
				tot += p->size;
				p = p->next;
			}
			if (p)
				return -tot;
 
			if (p == NULL)
			{
 
				tot = 0;
				p = current_node;
			  tot+=(q->size-rhs.position-1);
				q=q->next;
				while (q!=NULL)
				{
					if (p == q)
					{
						tot += (position+1);
						break;
					}
 
					tot += q->size;
					q = q->next;
				}
 
				if (q != NULL)
					return tot;
			}
 
			throw invalid_iterator();
		}
		iterator operator+=(const int &n)
		{
			(*this) = (*this) + n;
			return (*this);
		}
		iterator operator-=(const int &n)
		{
			(*this) = (*this) - n;
			return (*this);
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			iterator tmp(*this);
		 (*this) =(*this)+ 1;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator &operator++()
		{  *this=(*this)+1;
			return (*this);
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			iterator tmp(*this);
			 (*this) -= 1;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator &operator--()
		{
			*this=(*this)-1;
			return (*this);
		}
		/**
		 * TODO *it
		 */
		T &operator*() const
		{
       if(current_node==NULL)   throw invalid_iterator();
			 if (position<0||position>=current_node->size)  throw invalid_iterator();
			
 
			return *(current_node->data[position]);
		}
		/**
		 * TODO it->field
		 */
		T *operator->() const noexcept
		{
			 if(current_node==NULL)   throw invalid_iterator();
			 if (position<0||position>=current_node->size)  throw invalid_iterator();
			
			return current_node->data[position];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const 
		
		{
            return (position==rhs.position&&current_node==rhs.current_node);
 
		}
		bool operator==(const const_iterator &rhs) const {
          return (position==rhs.position&&current_node==rhs.current_node);
 
 
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			    return(position!=rhs.position||current_node!=rhs.current_node);
		}
		bool operator!=(const const_iterator &rhs) const {
			    return (position!=rhs.position||current_node!=rhs.current_node);
		}
	};
	class const_iterator
	{
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
  friend deque;
	friend iterator;
private:
		    int position;
				Node* current_node;
				
			 
	public:
		const_iterator():current_node(NULL),position(-1)
		{
		      
		}
 
		const_iterator&operator=(const const_iterator  &s){
            if(this ==&s )  return *this;
						 current_node=s.current_node;
						 position=s.position;
		}
		const_iterator(const const_iterator &other)
		{
			current_node=other.current_node;
			position=other.position;
		}
		const_iterator(const iterator &other)
		{
			 current_node=other.current_node;
			 position=other.position;
		}
		// And other methods in iterator.
		// And other methods in iterator.
		// And other methods in iterator.
			const_iterator operator+(const int &n) const
		{
          
 
            if(current_node==NULL)  throw invalid_iterator();
			if (n < 0)
				return (*this) - (-n);
			const_iterator tmp(*this);
		
	int counter = n;
				while (tmp.current_node->size - tmp.position <= counter)
				{
					counter -= tmp.current_node->size;
					if (tmp.current_node->next != NULL)tmp.current_node = tmp.current_node->next;
					else
					{
						counter += tmp.current_node->size;
						tmp.position += counter;
						return tmp;
					}
				}
				tmp.position += counter;
				return tmp;
 
 
		}
	const_iterator operator-(const int &n) const
		{
  
			if(current_node==NULL)  throw invalid_iterator();
			if (n < 0) return (*this) + (-n);
 
			int tot = 0;
		const_iterator tmp(*this);
		 	int c = n;
				while (tmp.position < c)
				{
					c -= (tmp.position + 1);
					if (tmp.current_node->prev != NULL)
					{
						tmp.current_node = tmp.current_node->prev;
						tmp.position = tmp.current_node->size - 1;
					}
					else
					{
				
						c += (tmp.position + 1);
						tmp.position -= c;
						return tmp;
				
					}
				}
				tmp.position -= c;
				return tmp;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
           if(current_node==NULL)  throw invalid_iterator();
			Node *p = current_node, *q = rhs.current_node;
			int tot = 0;
			if (p == q)
				return position - (rhs.position);
				tot+=(current_node->size-position-1);
				p=p->next;
			while (p != NULL)
			{
 
				if (p == q)
				{
					tot += rhs.position+1 ;
					break;
				}
				tot += p->size;
				p = p->next;
			}
			if (p)
				return -tot;
 
			if (p == NULL)
			{
 
				tot = 0;
				p = current_node;
			  tot+=(q->size-rhs.position-1);
				q=q->next;
				while (q!=NULL)
				{
					if (p == q)
					{
						tot += (position+1);
						break;
					}
 
					tot += q->size;
					q = q->next;
				}
 
				if (q != NULL)
					return tot;
			}
 
			throw invalid_iterator();
		}
	const_iterator operator+=(const int &n)
		{
			(*this) = (*this) + n;
			return (*this);
		}
	const_iterator operator-=(const int &n)
		{
			(*this) = (*this) - n;
			return (*this);
		}
		/**
		 * TODO iter++
		 */
	const_iterator operator++(int)
		{
		const_iterator tmp(*this);
		 (*this) =(*this)+ 1;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator &operator++()
		{  *this=(*this)+1;
			return (*this);
		}
		/**
		 * TODO iter--
		 */
	const_iterator operator--(int)
		{
			const_iterator tmp(*this);
			 (*this) -= 1;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator &operator--()
		{
			*this=(*this)-1;
			return (*this);
		}
		/**
		 * TODO *it
		 */
		T &operator*() const
		{
       if(current_node==NULL)   throw invalid_iterator();
			 if (position<0||position>=current_node->size)  throw invalid_iterator();
			
 
			return *(current_node->data[position]);
		}
		/**
		 * TODO it->field
		 */
		T *operator->() const noexcept
		{
			 if(current_node==NULL)   throw invalid_iterator();
			 if (position<0||position>=current_node->size)  throw invalid_iterator();
			
			return current_node->data[position];
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const 
		
		{
            return (position==rhs.position&&current_node==rhs.current_node);
 
		}
		bool operator==(const const_iterator &rhs) const {
          return (position==rhs.position&&current_node==rhs.current_node);
 
 
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			    return(position!=rhs.position||current_node!=rhs.current_node);
		}
		bool operator!=(const const_iterator &rhs) const {
			    return (position!=rhs.position||current_node!=rhs.current_node);
		}
	};
	/**
	 * TODO Constructors
	 */
	deque()
	{
 
		Node_size = 548;
		length = 0;
		head = rear =new Node(Node_size,NULL,NULL);
	}
	deque(const deque &other)
	{
 
	  Node_size=other.Node_size;
	  length=other.length;
 
		head= new Node(Node_size);
   head->size=other.head->size;
		if(length==0)  {rear=head;return;}
Node*p = other.head, *q = head;
			int pos = 0;
			while (p != NULL)
			{
				q->data[pos] = new T (*(p->data[pos]));
				++pos;
				if (pos == p->size)
				{
					p = p->next;
					if (p != NULL)
					{
						q = q->next = new Node(Node_size, q, NULL);
						q->size = p->size;
						pos = 0;
					}
				}
			}
 
 
 
	rear = q;
 
 
	}
	/**
	 * TODO Deconstructor
	 */
	~deque()
	{
			 selfclear();
			 //head=rear=new Node(Node_size);
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other)
	{
		if (this == &other)
			return *this;
   selfclear();
	 	 length=other.length; Node_size=other.Node_size;
		head= new Node(Node_size);
   head->size=other.head->size;
		if(length==0)  {rear=head;return(*this);}
Node*p = other.head, *q = head;
			int pos = 0;
			while (p != NULL)
			{
				q->data[pos] = new T (*(p->data[pos]));
				++pos;
				if (pos == p->size)
				{
					p = p->next;
					if (p != NULL)
					{
						q = q->next = new Node(Node_size, q, NULL);
						q->size = p->size;
						pos = 0;
					}
				}
			}
 
 
 
	rear = q;
 
		return (*this);
 
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T &at(const size_t &pos) {
     if(pos>=length)  throw index_out_of_bound();
	 
	 int tmp_position;
	 Node* tmp_node;
	 findposition(pos,tmp_node,tmp_position);
	 return*(tmp_node->data[tmp_position]);
	 
 
	}
	const T &at(const size_t &pos) const {
if(pos>=length||pos<0)  throw index_out_of_bound();
		 int tmp_position;
	 Node* tmp_node;
	 findposition(pos,tmp_node,tmp_position);
	 return *(tmp_node->data[tmp_position]);
	 
	}
	T &operator[](const size_t &pos) {
if(pos>=length||pos<0)  throw index_out_of_bound();
		 int tmp_position;
	 Node* tmp_node;
	 findposition(pos,tmp_node,tmp_position);
	 return *(tmp_node->data[tmp_position]);
	 
	}
	const T &operator[](const size_t &pos) const {
		if(pos>=length||pos<0)  throw index_out_of_bound();
		 int tmp_position;
	 Node* tmp_node;
	 findposition(pos,tmp_node,tmp_position);
	 return *(tmp_node->data[tmp_position]);
	 
		
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T &front() const {
      if(length==0)  throw container_is_empty();
	   return *(head->data[0]); 
	  	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T &back() const {
		  if(length==0)  throw container_is_empty();
	   return *(rear->data[rear->size-1]);
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
      iterator ite;
	     ite.current_node=head;
			 ite.position=0;
			return ite;
 
	}
	const_iterator cbegin() const {
		const_iterator itr;
			itr.current_node = head;
			itr.position = 0;
			return itr;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		iterator itr;
			itr.current_node = rear;
			itr.position = rear->size;
			return itr;
	}
	const_iterator cend() const {
				iterator itr;
			itr.current_node = rear;
			itr.position = rear->size;
			return itr;
 
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const
	{
		    return length==0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const
	{
		return static_cast<size_t> (length);
	}
	/**
	 * clears the contents
	 */
	void clear() {
		selfclear();
		head=rear=new Node(Node_size,NULL,NULL);
		length=0;
		return;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
  
	 if(pos.current_node==NULL)  throw invalid_iterator();
			if(pos.position<0||pos.position>pos.current_node->size)  throw invalid_iterator();
      Node* tmp=pos.current_node;
	while (tmp != NULL){
				if (tmp->next != NULL)
					tmp = tmp->next;
				else break;
	}
				if (tmp != rear)throw invalid_iterator();
	  
Node*p = pos.current_node;
				int nodePos = pos.position;
 
				//remain: insert end
				if (p == rear &&nodePos==Node_size)
				{
					p->next = new Node(Node_size, p, NULL);
				                rear = p->next;
					rear->data[0] = new T(value);
					rear->size = 1;
					length++;
					iterator itr(rear, 0);
					return itr;
				}
 
				//当前结点满
				if (p->size == Node_size)
				{
					p->next = new Node(Node_size, p, p->next);
					if (p != rear && p->next->next != NULL)p->next->next->prev = p->next;
					if (rear == p)rear = p->next;
 
					for (int i = nodePos; i < p->size; ++i)
					{
						p->next->data[i - nodePos] = p->data[i];
						p->data[i] = NULL;
					}
					p->data[nodePos] = new T(value);
					p->next->size = p->size - nodePos;
					p->size = nodePos + 1;
				}
				else//当前结点未满
				{
					for (int i = p->size; i > nodePos; --i)p->data[i] = p->data[i - 1];
					p->data[nodePos] = new T(value);
					p->size++;
				}
 
				length++;
 
				//reconstruct？
				iterator itr(p, nodePos);
				return itr;
 
 
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
     if(length==0)  throw container_is_empty();
		 if(pos.current_node==NULL)  throw invalid_iterator();
		if(pos.position<0||pos.position>=pos.current_node->size)   throw invalid_iterator();
		 
		 Node*tmp=pos.current_node;
	 while(tmp!=NULL){
			if(tmp==rear )  break;
			tmp=tmp->next;
		 } 
		 if(tmp==NULL)  throw invalid_iterator();
   
		 ////////////////////////do erases:.....
    --length;
		
 
		  Node* era_node=pos.current_node;
			int era_pos=pos.position;
		
      delete era_node->data[era_pos];
    	for (int i = era_pos; i < era_node->size - 1; ++i)
					era_node->data[i] =era_node->data[i + 1];
 
					--pos.current_node->size;
 
			if(era_node->size>0){
         if(era_pos==era_node->size){
              if(era_node==rear){
                  iterator ite;
									ite.current_node=rear;
									ite.position=rear->size;
							} 
							else{
								 iterator ite;
								 ite.current_node=era_node->next;
								 ite.position=0;
								 return ite;
							}     
             
				 }
 
       			iterator ite;
						 ite.current_node=era_node;
						 ite.position=era_pos;
             return ite;
		
 
				 }
 
             
			
 
			else if(era_node->size==0){
         /////////////  node_size==1  
               if(head==rear){
								 iterator ite;
								 ite.current_node=head;
								 ite.position=0;
								 return ite;
							 }
							 
							 if(era_node==rear){
                  rear=rear->prev;
									rear->next=NULL;
									delete era_node;
									return end();
							 }
							  if(era_node==head){
                  Node*tmp=head->next;
							
									head=tmp;
									head->prev=NULL;
 
									delete era_node;
 
									return begin();
							 }
 
							 
								    iterator ite;
										ite.current_node=era_node->next;
										ite.position=0;
										era_node->next->prev=era_node->prev;
										era_node->prev->next=era_node->next;
										delete era_node;
										return ite;
							 
							
 
			}
		 
 
 
 
 
 
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
		
		if(rear->size==Node_size){
         rear->next=new Node(Node_size,rear,NULL);
				 rear=rear->next;
				 rear->size=1;
				 rear->data[0]=new T(value);
				 ++length;
				 return ;
		}
		
			++rear->size;
			rear->data[rear->size-1]=new T(value);
			++length;
		
 
 
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
 
 
	/*  */
	void pop_back() {
  if(length==0)   throw container_is_empty();
	length--;
   //if(rear->size>1){
	       
		     delete rear->data[rear->size-1];
					rear->data[rear->size-1]=NULL;
				rear->size--;	
			 
	 //}
	 //else {
       if(rear->size==0&&rear!=head){
 
		Node*tmp=rear;
		rear=rear->prev;
		rear->next=NULL;
		delete tmp;
			 }
				
		    
	 
 
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
 
++length;
if(head->size==Node_size){
	head=new Node(Node_size,NULL,head);
	head->next->prev=head;
	head->data[0]=new T(value);
	head->size=1;
	return ;
}
		///// 头节点未满
	
          for(int i=head->size;i>0;--i){
						 head->data[i]=head->data[i-1];
					}
					head->data[0]=new T(value);
					 head->size++;
 
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
 
  if(length==0) throw container_is_empty();
	length--;
 
 
	delete head->data[0];
	
 
		   for(int i=0;i<head->size-1;++i){
				 //if(i==0) head->data[i]=new T(*(head->data[i+1]));
				 head->data[i]=head->data[i+1];
			 }
			 head->data[head->size-1]=NULL;
			head->size--;
		
	 
	 if(head->size==0&&head!=rear){
      Node*tmp=head;
			head=head->next;
			head->prev=NULL;
	    delete tmp;
 
	 }
 
 
 
	}
};
 
} // namespace sjtu
 
#endif
