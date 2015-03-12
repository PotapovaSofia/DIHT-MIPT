//#include <stdafx.h>
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cassert>

using namespace std;

const long maxn=524290;

long h[maxn];
bool mark[maxn];
long tree[maxn*4];
vector < pair <long,long> > query;
long sz,dvk;
long start[maxn];
vector <long> list;
vector <long> edges[maxn];

void dfs(long v) {
	mark[v]=true;
	list.push_back(v);
	for (long j=0;j<edges[v].size();j++) {
		long u=edges[v][j];
		if (!mark[u]) {
			h[u]=h[v]+1;
			dfs(u);
			list.push_back(v);
		}
	}
}

void build(long i,long l,long r) {
	if (l==r) {
		if (l<sz) tree[i]=list[l];
		else tree[i]=dvk+1;
		return;
	}
	long middle=(l+r)/2;
	build(i*2,l,middle);
	build(i*2+1,middle+1,r);
	if (h[tree[i*2]]<h[tree[i*2+1]])
		tree[i]=tree[i*2];
	else tree[i]=tree[i*2+1];
}

long get_min(long i,long tl,long tr,long l,long r) {
	if ((tl==l)&&(tr==r))
		return(tree[i]);
	long middle=(tl+tr)/2;
	if (r<=middle)
		return(get_min(i*2,tl,middle,l,r));
	else if (l>middle)
		return(get_min(i*2+1,middle+1,tr,l,r));
	else {
		long t1=get_min(i*2,tl,middle,l,middle);
		long t2=get_min(i*2+1,middle+1,tr,middle+1,r);
		if (h[t1]<h[t2])
			return(t1);
		else return(t2);
	}
}

void prepare() {
	list.resize(1);
	list[0]=-1;
	dfs(1);

	for (long j=1;j<list.size();j++)
		if (start[list[j]]==0)
			start[list[j]]=j;

	sz=list.size();
	dvk=1;
	while(sz-1>dvk)
		dvk=dvk*2;
	h[dvk+1]=1e9;
	build(1,1,dvk);
}

int main() {
	long N;
	scanf("%ld",&N);
	for (long it=1;it<=N;it++) {
		long x,y;
		scanf("%ld%ld",&x,&y);
		edges[y+1].push_back(x+1);
	}
	long M;
	scanf("%ld",&M);
	for (long it=1;it<=M;it++) {
		long a,b;
		scanf("%ld%ld",&a,&b);
		query.push_back(make_pair(a+1,b+1));
	}
	prepare();
	for (long j=0;j<query.size();j++) {
		long t1=start[query[j].first];
		long t2=start[query[j].second];
		if (t1>t2)
			swap(t1,t2);
		long ans=get_min(1,1,dvk,t1,t2);
		printf("%ld\n",ans);
	}

}