#include <stdlib.h>
#include "hashMap.h"
#include "string.h"

int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i];
	return r;
}

void initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

void freeMap (struct hashMap * ht)
{
	int i;
	struct hashLink *temp;
	struct hashLink *temp2;
	for(i=0; i<ht->tableSize; i++){
		temp=ht->table[i];
		while(temp!=0){
			temp2=temp->next;
			free(temp->key);
			free(temp);
			temp=temp2;
		}
	}
	free(ht->table);
	ht->count=0;
	ht->table=0;
	ht->tableSize=0;	
}

void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{
	int bucket = stringHash2(k)%ht->tableSize;
	struct hashLink * newLink; 
	struct hashLink *temp;
	if(ht->table[bucket] == 0){
		newLink = (struct hashLink *) malloc(sizeof(struct hashLink));
		ht->table[bucket]=newLink;
		ht->table[bucket]->key=k;
		ht->table[bucket]->value=v;
		ht->table[bucket]->next=0;
		ht->count++;
	}else{
		temp=ht->table[bucket];
		while(temp!=0){
			if(strcmp(temp->key,k)==0){
				temp->value=v;
				return;
			}else{
				temp=temp->next;
			} 
		}
		newLink = (struct hashLink *) malloc(sizeof(struct hashLink));
		newLink->key=k;
		newLink->value=v;
		newLink->next=ht->table[bucket];
		ht->table[bucket]=newLink;
		ht->count++;
	}
}

ValueType* atMap (struct hashMap * ht, KeyType k)
{
	int bucket = stringHash2(k)%ht->tableSize;
	struct hashLink *cur;
	struct hashLink *prev;
	if(ht->table[bucket] == 0){
		return 0;
	}else{
		cur=ht->table[bucket];
		prev=ht->table[bucket];
		while(cur!=0){
			if(strcmp(cur->key,k)==0){
				return &(cur->value);  
			}else{
				prev=cur;
				cur=cur->next;
			} 
		}
		return 0;
	}
}

int containsKey (struct hashMap * ht, KeyType k)
{
	if (atMap(ht,k)==0){
		return 0;
	}else{
		return 1;
	}
}

void removeKey (struct hashMap * ht, KeyType k)
{
	int bucket = stringHash2(k)%ht->tableSize;
	struct hashLink *cur; 
	struct hashLink *prev;  
	if(ht->table[bucket] == 0){
		return;
	}else{
		cur=ht->table[bucket];
		prev=ht->table[bucket];
		while(cur!=0){
			if(strcmp(cur->key,k)==0){
				if(cur==ht->table[bucket]){
					ht->table[bucket]=cur->next;
					
				}else{
				prev->next=cur->next;
				}
				free(cur);
				ht->count--;
				return;
			}else{
				prev=cur;
				cur=cur->next;
			} 
		}
	}
}

int size (struct hashMap *ht)
{
	return ht->count;
}

int capacity(struct hashMap *ht)
{
	return ht->tableSize;
}

int emptyBuckets(struct hashMap *ht)
{
	int runner = 0;
	int i;
	for(i=0; i<ht->tableSize; i++){
		if(ht->table[i]==0){
			runner++;
		}
	}
	return runner;
}

float tableLoad(struct hashMap *ht)
{
	return (float)size(ht)/(float)capacity(ht);
}
