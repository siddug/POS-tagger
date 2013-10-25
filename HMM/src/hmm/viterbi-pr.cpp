#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <string.h>
using namespace std;

typedef struct Node Node;
typedef Node* NodePtr;

struct Node{
	int state;
	NodePtr prev_node;
	float prob;
	Node(int s,NodePtr prev, float p):state(s),prev_node(prev),prob(p){}
};

char* speech;
vector<char> correct_pos;
vector<string> tokens;
int count;
map<string,int> word_id_map;

int v_correct,a_correct,r_correct,n_correct,o_correct;
int v_assigned,a_assigned,r_assigned,n_assigned,o_assigned;
int v_total,a_total,r_total,n_total,o_total;

int tgs[5][5];

int printPath(NodePtr node){
	int prev_count=0;
	if(node->prev_node->prev_node!=NULL){
		prev_count = printPath(node->prev_node);
	}
	cout<<tokens[count]<<"_"<<speech[node->state]<<" ";

        int ti, tj;
        if(speech[node->state]=='V'){ti=0;}
        else if(speech[node->state]=='A'){ti=1;}
        else if(speech[node->state]=='R'){ti=2;}
        else if(speech[node->state]=='N'){ti=3;}
        else {ti=4;}
        if(correct_pos[count]=='V'){tj=0;}
        else if(correct_pos[count]=='A'){tj=1;}
        else if(correct_pos[count]=='R'){tj=2;}
        else if(correct_pos[count]=='N'){tj=3;}
        else {tj=4;}
        
        if(ti==tj){tgs[tj][tj]++;}
        else{
            tgs[tj][ti]++;
        }
        
	if(speech[node->state]=='V')v_assigned++;
	else if(speech[node->state]=='A')a_assigned++;
	else if(speech[node->state]=='R')r_assigned++;
	else if(speech[node->state]=='N')n_assigned++;
	else if(speech[node->state]=='O')o_assigned++;

	if(speech[node->state]==correct_pos[count]){
		prev_count++;
		if(correct_pos[count]=='V')v_correct++;
		else if(correct_pos[count]=='A')a_correct++;
		else if(correct_pos[count]=='R')r_correct++;
		else if(correct_pos[count]=='N')n_correct++;
		else if(correct_pos[count]=='O')o_correct++;
	}
	count++;
	return prev_count;
}

/*void printPath(NodePtr node){
	//if(node==NULL)cout<<"Node Null at : "<<count<<endl;
	//if(node->prev_node==NULL)cout<<"Previous node null at : "<<count<<endl;
	if(node->prev_node->prev_node!=NULL){printPath(node->prev_node);}
	//cout<<"Entered node : "<<count<<endl;
	cout<<tokens[count]<<"_"<<speech[node->state]<<" ";count++;
}*/

int main(){
    
    for(int temp=0;temp<5;temp++){
        for(int temp2=0;temp2<5;temp2++){
            tgs[temp][temp2]=0;
        }
    }
	int num_states;
	int num_alpha;
	int total_tags = 0;
	int total_correct_tags = 0;

	v_correct=0;a_correct=0;r_correct=0;n_correct=0;o_correct=0;
	v_total=0;a_total=0;r_total=0;n_total=0;o_total=0;
	v_assigned=0;a_assigned=0;r_assigned=0;n_assigned=0;o_assigned=0;

	cin>>num_states>>num_alpha;
	//cout<<"Number of states: "<<num_states<<endl;
	//cout<<"Possibilities: "<<num_alpha<<endl;
	float state_trans_prob[num_states+1][num_states+1];
	float alpha_prob[num_states+1][num_alpha+1];
	speech=new char[6];
	speech[0]='s';speech[1]='V';speech[2]='A';speech[3]='R';speech[4]='N';speech[5]='O';
	

	/*Taking input of probabilities of transitions to start state */
	for(int i=1;i<=num_states;i++){
		cin>>state_trans_prob[0][i];
	}
	
	/*Taking input of probabilities of transitions from one state to another */
	for(int i=1;i<=num_states;i++){
		for(int j=1;j<=num_states;j++){
			cin>>state_trans_prob[i][j];
		}
	}
	
	/*Taking input of probabilities of taking an alphabet from a state */
	for(int i=1;i<=num_alpha;i++){
		float total=0;
		int word_id;cin>>word_id;
		string word;cin>>word;
		//cout<<word_id<<" "<<word<<endl;
		word_id_map[word]=word_id;
		for(int j=1;j<=num_states;j++){
			cin>>alpha_prob[j][word_id];
			total+=alpha_prob[j][word_id];
		}
		//if(total!=1)cout<<"error"<<endl;
	}

	/*Taking input of observed sequence */
	char sentence[1024];
	cin.ignore();
	cin.ignore();
        cin.getline(sentence,1024);
        /*string s;
        cin>>s;
        for(int ik=0;ik<1024&&ik<s.size();ik++)
        {sentence[ik]=s[ik];}
        sentence[ik+1]='\0';*/
        int k=100;
	while(k!=0){
            
            k--;
		//cout<<"Given: "<<sentence<<endl;
		tokens.clear();
		correct_pos.clear();
		string token="";
		count=0;
		for(int i=0;sentence[i]!='\0';i++){
			if(sentence[i]!='_')token.push_back(sentence[i]);
			else{
				if(token!=""){
					tokens.push_back(token);
					total_tags++;
					if(sentence[i+1]=='V')v_total++;
					else if(sentence[i+1]=='A')a_total++;
					else if(sentence[i+1]=='R')r_total++;
					else if(sentence[i+1]=='N')n_total++;
					else if(sentence[i+1]=='O')o_total++;
					correct_pos.push_back(sentence[i+1]);
				}
				token="";
				i++;i++;
				if(sentence[i]=='\0')i--;
			}
		}
		//tokens.push_back(token);
		int num_obs = tokens.size();
		int obs_seq[num_obs];
		for(int i=0;i<num_obs;i++){
			if(word_id_map.count(tokens[i])>0)obs_seq[i] = word_id_map[tokens[i]];
			else obs_seq[i]=-1;
			//if(obs_seq[i]==-1)cout<<"NotFound:";
			//cout<<tokens[i]<<" ";
		}
		//cout<<endl;
		
		/* cur_states stores the possible paths ending at different states */
		NodePtr cur_states[num_states+1];
		
		/*Initialising with the first transition */
		NodePtr start_node = new Node(0,NULL,1);
		for(int i=1;i<=num_states;i++){
			float prob = state_trans_prob[0][i];
			cur_states[i] = new Node(i,start_node,prob);
			//cout<<i<<": "<<prob<<", ";
		}
		//cout<<endl;
		
		NodePtr temp_states[num_states+1][num_states+1];
		for(int i=0;i<num_obs;i++){
			//cout<<"Looking for "<<tokens[i]<<endl;
			for(int j=1;j<=num_states;j++){
				for(int k=1;k<=num_states;k++){
					//if(cur_states[j]->prob==0)cout<<"cur_states[j]->prob"<<endl;
					//if(state_trans_prob[cur_states[j]->state][k]==0)cout<<"state_trans_prob[cur_states[j]->state][k]"<<endl;
					//if(alpha_prob[cur_states[j]->state][obs_seq[i]]==0)cout<<"alpha_prob[cur_states[j]->state][obs_seq[i]]"<<endl;
					float temp_prob = 0.2;
					if(obs_seq[i]!=-1)temp_prob = alpha_prob[cur_states[j]->state][obs_seq[i]];
					float prob = cur_states[j]->prob * state_trans_prob[cur_states[j]->state][k] * temp_prob;
					temp_states[j][k] = new Node(k,cur_states[j],prob);
					//cout<<cur_states[j]->state<<","<<k<<": "<<prob<<", ";
				}
				//cout<<endl;
			}
			for(int j=1;j<=num_states;j++){
				NodePtr highest_node = temp_states[1][j];
				for(int k=1;k<=num_states;k++){
					if(temp_states[k][j]->prob > highest_node->prob)highest_node = temp_states[k][j];
				}
				cur_states[j] = highest_node;
				//cout<<cur_states[j]->state<<": "<<cur_states[j]->prob<<", ";
			}
			//cout<<endl<<endl;
		}
		
		NodePtr end_node = cur_states[1];
		for(int i=1;i<=num_states;i++){
			if(cur_states[i]->prob > end_node->prob)end_node = cur_states[i];
		}
		//cout<<"Printing path"<<endl;
		//cout<<end_node->state<<endl;
		//cout<<end_node->prob<<endl;
		//cout<<end_node->prev_node->state<<endl;
		int correct_tags = printPath(end_node->prev_node);
		total_correct_tags+=correct_tags;
		cout<<endl;
		cin.getline(sentence,1024);
	}
	cout<<"Correct: "<<total_correct_tags<<endl;
	cout<<"Given: "<<total_tags<<endl;
        float t_r=0;
        float t_p=0;
	cout<<"V statistics"<<endl;
	float Recall=(float)v_correct/v_total;
	float Precision=(float)v_correct/v_assigned;
	cout<<"Precision: "<<Precision<<endl;
	cout<<"Recall: "<<Recall<<endl;
	cout<<"F: "<<(2*Precision*Recall)/(Precision+Recall)<<endl;
	cout<<endl;
	t_r+=Recall;
        t_p+=Precision;
	cout<<"A statistics"<<endl;
	Recall=(float)a_correct/a_total;
	Precision=(float)a_correct/a_assigned;
	cout<<"Precision: "<<Precision<<endl;
	cout<<"Recall: "<<Recall<<endl;
	cout<<"F: "<<(2*Precision*Recall)/(Precision+Recall)<<endl;
	cout<<endl;
	t_r+=Recall;
        t_p+=Precision;
	cout<<"R statistics"<<endl;
	Recall=(float)r_correct/r_total;
	Precision=(float)r_correct/r_assigned;
	cout<<"Precision: "<<Precision<<endl;
	cout<<"Recall: "<<Recall<<endl;
	cout<<"F: "<<(2*Precision*Recall)/(Precision+Recall)<<endl;
	cout<<endl;
	t_r+=Recall;
        t_p+=Precision;
	cout<<"N statistics"<<endl;
	Recall=(float)n_correct/n_total;
	Precision=(float)n_correct/n_assigned;
	cout<<"Precision: "<<Precision<<endl;
	cout<<"Recall: "<<Recall<<endl;
	cout<<"F: "<<(2*Precision*Recall)/(Precision+Recall)<<endl;
	cout<<endl;
	t_r+=Recall;
        t_p+=Precision;
	cout<<"O statistics"<<endl;
	Recall=(float)o_correct/o_total;
	Precision=(float)o_correct/o_assigned;
	cout<<"Precision: "<<Precision<<endl;
	cout<<"Recall: "<<Recall<<endl;
	cout<<"F: "<<(2*Precision*Recall)/(Precision+Recall)<<endl;
	cout<<endl;
	t_r+=Recall;
        t_p+=Precision;
	int total_correct = v_correct+a_correct+r_correct+n_correct+o_correct;
	int total_total = v_total+a_total+r_total+n_total+o_total;
	int total_assigned = v_assigned+a_assigned+r_assigned+n_assigned+o_assigned;
        t_r/=5;
        t_p/=5;
	cout<<"Overall statistics"<<endl;
	Recall=(float)total_correct/total_total;
	Precision=(float)total_correct/total_assigned;
	//cout<<"Precision: "<<Precision<<endl;
	//cout<<"Recall: "<<Recall<<endl;
	cout<<"Precision: "<<t_p<<endl;
	cout<<"Recall: "<<t_r<<endl;
	//cout<<"F: "<<(2*Precision*Recall)/(Precision+Recall)<<endl;
	cout<<"F: "<<(2*t_p*t_r)/(t_p+t_r)<<endl;
	cout<<endl;
        
        cout<<"Confusion matrix: "<<endl;
        for(int temp=0;temp<5;temp++){
            
            int c=0;
                for(int temp2=0;temp2<5;temp2++){
                        c+=tgs[temp][temp2];
                }
            for(int temp2=0;temp2<5;temp2++){
                        cout<<(float)tgs[temp][temp2]/(float)c<<" ";
                }
            cout<<endl;
        }
        
	return 0;
}



	
	
