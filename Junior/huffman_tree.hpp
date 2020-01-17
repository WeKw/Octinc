/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_HUFFMAN_TREE__
#define OCTINC_HUFFMAN_TREE__

#include <queue>
#include <cstdint>
#include <vector>
#include <stack>

namespace Octinc
{
	class huffman_tree
	{
		public:
			struct huffman_code
			{
				uint8_t word;
				uint8_t len;
				uint32_t code;// Up to 32 bits, otherwise amendments will be implemented.
			};
			
			huffman_tree(){}
			~huffman_tree()
			{
				clear();
			}
			
			void build(uint8_t* s,size_t siz)
			/*
			Build a Huffman tree with the array of words.
			*/
			{
				int bset[256]={0};
				
				for(size_t i=0;i<siz;++i)
					++bset[s[i]];
				
				std::vector<huffman_node*>nodes;
				
				for(size_t i=0;i<siz;++i)
					if(bset[i]>0)
					{
						huffman_node* p=new huffman_node;
						p->fs=bset[i];
						p->value=i;
						nodes.push_back(p);
					}
					
				_root=build(nodes);
			}
			
			huffman_code* get_table()
			/*
			Get the Huffman code table,
			and return the pointer of the table.
			*/
			{
				if(_root=nullptr)
					return nullptr;
			
				huffman_node* p=_root;
				std::stack<huffman_node*>s;
				
				while(!s.empty()||p)
				{
					while(p)
					{
						s.push(p);
						
						if(p->is_leaf())
						{
							_table[p->value].word=p->value;
							_table[p->value].len=p->len;
							_table[p->value].code=p->precode;
						}
						
						huffman_node* par=p;
						p=p->left;
						
						if(p)
						{
							p->precode=par->precode<<1;
							p->len=par->len+1;
						}
					}
					
					if(!s.empty())
					{
						p=s.top();
						s.pop();
						huffman_node* par=p;
						
						if(p)
						{
							p->precode=(par->precode<<1)|1;
							p->len=par->len+1;
						}
					}
				}
				
				return _table;
			}
			
			huffman_code* canonical()
			/*
			Get the canonical Huffman code table.
			*/
			{
				class minivar
				{
					public:
						bool operator()(const huffman_code* left,const huffman_code* right)
						{
							return left->len<right->len;
						}	
				};
				
			    std::priority_queue<huffman_code*,std::vector<huffman_code*>,minivar>h;
				
				for(int i=0;i<256;++i)
					if(_table[i].len>0)
						h.push(&_table[i]);
					
				huffman_code* pre=nullptr;
				
				while(h.empty())
				{
					huffman_code* p=h.top();
					
					if(!pre)
					{
						p->code=0;
						pre=p;
					}
					else if(pre->len==p->len)
					{
						p->code=pre->code+1;
						pre=p;
					}
					else if(pre->len<p->len)
					{
						p->code=(pre->code+1)<<2;
						pre=p;
					}
					
					h.pop();
				}
				
				return _table;
			}
			
			void clear()
			{
				if(_root==nullptr)
					return;
				
				huffman_node* p=_root;
				std::stack<huffman_node*>s;
				
				while(!s.empty()||p)
				{
					while(p)
					{
						s.push(p);
						p=p->left;
					}
					
					if(!s.empty())
					{
						p=s.top();
						s.pop();
						
						huffman_node* par=p;
						p=p->right;
						delete par;
					}
				}
			}
		
		protected:
			class huffman_node
			{
				public:
					int value;
					double fs;
					uint32_t precode;
					uint8_t len;
					huffman_node *left,*right;
					huffman_node():fs(0),left(nullptr),right(nullptr),len(0),precode(0){}
					inline bool is_leaf() const
					{
						return !left&&!right;
					}
			};		
	
			huffman_node* _root;
			huffman_code _table[256];
			
			
			huffman_node* build(std::vector<huffman_node*>a)
			{
				size_t siz=a.size();
				
				class minivar
				{
					public:
						bool operator()(const huffman_node* left,const huffman_node* right)
						{
							if(left->fs<right->fs)
								return true;
							
							if((left->fs==right->fs)&&(left->is_leaf()))
								return true;
							
							return false;
						}	
				};
				
				std::priority_queue<huffman_node*,std::vector<huffman_node*>,minivar>h;
				
				for(size_t i=0;i<siz;++i)
				{
					huffman_node* p=new huffman_node;
					p=a[i];
					h.push(p);
				}
				
				while(h.size()>1)
				{
					huffman_node* l=h.top();
					h.pop();
					huffman_node* r=h.top();
					h.pop();
					
					huffman_node* n=new huffman_node;
					n->fs=l->fs+r->fs;
					n->left=l;
					n->right=r;
					h.push(n);
				}
				
				return h.top();
			}
	};
}

#endif
