#include<bits/stdc++.h>
using namespace std;
#define ll long long int
#define ld long double
#define rep(n) for(ll i=0;i<(n);i++)
#define jrep(n) for(ll j=0;j<(n);j++)
#define all(a) (a).begin(), (a).end()
#define dpp(a) cout<<(a).first<<" "<<(a).second<<endl;
#define append push_back
#define mod 1000000007
#define ffast ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
#define fast ios::sync_with_stdio(0);
ll pw(ll a,ll p, ll m=mod){if(p==1)return a;ll t=pw(a,p/2);
t=(m+(t*t)%m)%m;if(p%2!=0)t*=a;return (t+m)%m; } vector<ll> 
uniq(vector<ll> arr){vector<ll>brr;sort(all(arr));brr.append(arr[0]); 
for(auto i:arr)if(brr.back()!=i)brr.append(i);return brr;}


void solve(){
    string s; cin>>s; cout<<s<<endl;
}

int main(){
    fast
    ll t=1;
    //cin>>t;
    rep(t) solve();
}
