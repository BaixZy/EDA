#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Baixz


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
   
   const vector<Dir> dirs = {Up,Down,Left,Right}; //Directions of movements.
   
   struct Pathinfo {
   	Pos actual;
   	Pos previous;
   	Dir direction;
   };
   
   
   typedef vector<vector<bool>> Vis; //Mark
   typedef vector<vector<Pathinfo>> Path; //First pos = opbjetive Second pos = previous
      
   Dir calcul_direction(const Pos& current, Pos& objective, Path& p) {
  	//Look for the objectivce and then go back to look the direction of movement.
  	Dir ret = Up; //It doesn't matter.
  	int i = objective.i;
  	int j = objective.j;
  	while (p[i][j].actual != current) {
  		ret = p[i][j].direction;
  		Pos previous = p[i][j].previous;//Go to previous one
  		i = previous.i;
  		j = previous.j;
  	}
  	return ret; 	
   }
   
   bool search_weapons(const Pos& player, Vis& visit, Pos& objective, Path& p, const int my_id) {
   	queue<pair<Pos,int>> container;
   	container.push({player,0}); //Distance 0.
   	bool found_gun = false;
   	bool first = true;
   	while (not container.empty()) {
   		Pos aux = container.front().first; // Get the position
   		int distance_frombeg = container.front().second; //Get the distance.
   		container.pop();
   		int i = aux.i; //Coordenadas
   		int j = aux.j;
   		visit[i][j] = true;
   		if (first) {
   			first = false;
   			p[i][j].actual = aux;
   			p[i][j].previous = aux;
   			p[i][j].direction = Up; //It doesn't matter this initial position.
   		}
   		found_gun = (cell(aux).weapon == Bazooka or cell(aux).weapon == Gun);
   		if (found_gun) {
   		    	objective = aux;
   		   	return true;  
   		}
      		if (distance_frombeg < 25) {	//20 distance max from the beginning	  		  		  		
   			for (Dir d: dirs) { //In case we find a gun 	
   				Pos aux2 = aux+d;
   				i = aux2.i;
   				j = aux2.j;	     		    
   		    		//Open Street
   		    		if (pos_ok(aux2) and not visit[i][j] and  cell(aux2).id == -1 and cell(aux2).resistance == -1 and cell(aux2).type == Street) {
   		    			p[i][j].actual = aux2; //Introduce the new destination.
   		    			p[i][j].previous = aux; //Introduce the previous position.
   		    			p[i][j].direction = d; //Introduce the direction of movement to reach this new position.
   		    			container.push({aux2,distance_frombeg + 1});		    
   				}
   				//I have to pass my own barricade
   				else if (pos_ok(aux2) and not visit[i][j] and  cell(aux2).id == -1 and cell(aux2).resistance != -1 and cell(aux2).b_owner == my_id) {
   					p[i][j].actual = aux2; //Introduce the new destination.
   		    			p[i][j].previous = aux; //Introduce the previous position.
   		    			p[i][j].direction = d; //Introduce the direction of movement to reach this new position.
   		    			container.push({aux2,distance_frombeg + 1});	
   				}
   			}
   	        }
 	}
 	return false;
}
  
  bool search_loot(const Pos& player, Vis& visit, Pos& objective, Path& p, const int my_id) {
  	queue<pair<Pos,int>> container;
   	container.push({player,0}); //Distance 0.
   	bool found = false;
   	bool first = true;
   	while (not container.empty()) {
   		Pos aux = container.front().first; // Get the position
   		int distance_frombeg = container.front().second; //Get the distance.
   		container.pop();
   		int i = aux.i; 
   		int j = aux.j;
   		visit[i][j] = true;
   		if (first) {
   			first = false;
   			p[i][j].actual = aux;
   			p[i][j].previous = aux;
   			p[i][j].direction = Up; //It doesn't matter this initial position.
   		}
   		found = (cell(aux).bonus == Money or cell(aux).bonus == Food);
   		if (found) {
   			objective = aux;
   			return true;  		
   		}
   		if (distance_frombeg < 20) {   					  		
   			for (Dir d: dirs) { //In case we find a gun 	
   				Pos aux2 = aux+d;
   				i = aux2.i;
   				j = aux2.j;	     		    
   		   		 //Open Street
   		    		if (pos_ok(aux2) and not visit[i][j] and  cell(aux2).id == -1 and cell(aux2).resistance == -1 and cell(aux2).type == Street) {
   		    			p[i][j].actual = aux2; //Introduce the new destination.
   		    			p[i][j].previous = aux; //Introduce the previous position.
   		    			p[i][j].direction = d; //Introduce the direction of movement to reach this new position.
   		    			container.push({aux2,distance_frombeg + 1});		    
   				}
   				//I have to pass my own barricade
   				else if (pos_ok(aux2) and not visit[i][j] and  cell(aux2).id == -1 and cell(aux2).resistance != -1 and cell(aux2).b_owner == my_id) {
   					p[i][j].actual = aux2; //Introduce the new destination.
   		    			p[i][j].previous = aux; //Introduce the previous position.
   		    			p[i][j].direction = d; //Introduce the direction of movement to reach this new position.
   		    			container.push({aux2,distance_frombeg + 1});	
   				}
   			}
   		}
   	}
       return false;
  }
  
  bool search_enemies(const Pos& player, Vis& visit, Pos& objective, Path& p, const int my_id, const vector<Dir>& vector, const bool& bazooka, const bool& gun) {
  	queue<pair<Pos,int>> container;
   	container.push({player,0}); //Distance 0.
   	bool first = true;
   	while (not container.empty()) {
   		Pos aux = container.front().first; // Get the position
   		int distance_frombeg = container.front().second; //Get the distance.
   		container.pop();
   		int i = aux.i; 
   		int j = aux.j;
   		visit[i][j] = true;
   		if (first) {
   			first = false;
   			p[i][j].actual = aux;
   			p[i][j].previous = aux;
   			p[i][j].direction = Up; //It doesn't matter this initial position.
   		}
   	for (Dir d : vector) {
            Pos neighbor = aux + d;
            if (bazooka) {
            	if (pos_ok(neighbor) and cell(neighbor).id != -1 and citizen(cell(neighbor).id).player != my_id and (citizen(cell(neighbor).id).weapon == Gun or citizen(cell(neighbor).id).weapon == Hammer or citizen(cell(neighbor).id).weapon == NoWeapon))  {
               	 	// Enemy troop found, set as objective
                	objective = neighbor;
               	 	int i = objective.i;
                	int j = objective.j;
                	p[i][j].actual = objective;
                	p[i][j].previous = aux;
                	p[i][j].direction = d;
                	return true;
            	}
            }
            else if (gun) {
            if (pos_ok(neighbor) and cell(neighbor).id != -1 and citizen(cell(neighbor).id).player != my_id and (citizen(cell(neighbor).id).weapon == Hammer or citizen(cell(neighbor).id).weapon == NoWeapon))  {
               	 	// Enemy troop found, set as objective
                	objective = neighbor;
               	 	int i = objective.i;
                	int j = objective.j;
                	p[i][j].actual = objective;
                	p[i][j].previous = aux;
                	p[i][j].direction = d;
                	return true;
            	}
            
            }
            else {
            if (pos_ok(neighbor) and cell(neighbor).id != -1 and citizen(cell(neighbor).id).player != my_id and citizen(cell(neighbor).id).weapon == NoWeapon)  {
               	 	// Enemy troop found, set as objective
                	objective = neighbor;
               	 	int i = objective.i;
                	int j = objective.j;
                	p[i][j].actual = objective;
                	p[i][j].previous = aux;
                	p[i][j].direction = d;
                	return true;
            	}
            } 
            
        }
   		if (distance_frombeg < 20) {   					  		
   			for (Dir d: dirs) { //In case we find a gun 	
   				Pos aux2 = aux+d;
   				i = aux2.i;
   				j = aux2.j;	     		    
   		    		//Open Street
   		    		if (pos_ok(aux2) and not visit[i][j] and  cell(aux2).id == -1 and cell(aux2).resistance == -1 and cell(aux2).type == Street) {
   		    			p[i][j].actual = aux2; //Introduce the new destination.
   		    			p[i][j].previous = aux; //Introduce the previous position.
   		    			p[i][j].direction = d; //Introduce the direction of movement to reach this new position.
   		    			container.push({aux2,distance_frombeg + 1});		    
   				}
   				//I have to pass my own barricade
   				else if (pos_ok(aux2) and not visit[i][j] and  cell(aux2).id == -1 and cell(aux2).resistance != -1 and cell(aux2).b_owner == my_id) {
   					p[i][j].actual = aux2; //Introduce the new destination.
   		    			p[i][j].previous = aux; //Introduce the previous position.
   		    			p[i][j].direction = d; //Introduce the direction of movement to reach this new position.
   		    			container.push({aux2,distance_frombeg + 1});	
   				}
   			}
   		}
      }
      return false;
  }
  
  bool search_barricades(const Pos& player, Vis& visit, Pos& objective, Path& p, const int my_id, const vector<Dir>& vector) {
  	queue<pair<Pos,int>> container;
   	container.push({player,0}); //Distance 0.
   	bool first = true;
   	while (not container.empty()) {
   		Pos aux = container.front().first; // Get the position
   		int distance_frombeg = container.front().second; //Get the distance.
   		container.pop();
   		int i = aux.i; 
   		int j = aux.j;
   		visit[i][j] = true;
   		if (first) {
   			first = false;
   			p[i][j].actual = aux;
   			p[i][j].previous = aux;
   			p[i][j].direction = Up; //It doesn't matter this initial position.
   		}
   		
   	//Caso Base	
   	for (Dir d: vector) {   
   		 Pos neighbor = aux + d;		
   		if (pos_ok(neighbor) and cell(neighbor).resistance != -1 and cell(neighbor).b_owner == my_id and cell(neighbor).id == -1)  {
               	 	// Enemy troop found, set as objective
               	 	objective = neighbor;
               	 	int i = objective.i;
                	int j = objective.j;
                	p[i][j].actual = objective;
                	p[i][j].previous = aux;
                	p[i][j].direction = d;
                	return true;
            	}
        }	
   			
        if (distance_frombeg < 20) {   					  		
   			for (Dir d: dirs) { //In case we find a gun 	
   				Pos aux2 = aux+d;
   				i = aux2.i;
   				j = aux2.j;	     		    
   		    		//Open Street
   		    		if (pos_ok(aux2) and not visit[i][j] and  cell(aux2).id == -1 and cell(aux2).resistance == -1 and cell(aux2).type == Street) {
   		    			p[i][j].actual = aux2; //Introduce the new destination.
   		    			p[i][j].previous = aux; //Introduce the previous position.
   		    			p[i][j].direction = d; //Introduce the direction of movement to reach this new position.
   		    			container.push({aux2,distance_frombeg + 1});		    
   				}
   			}
   	}
   }
  return false;
}
  
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {    
   	int r = board_rows();
   	int c = board_cols();
   	Vis mark(r, vector<bool>(c,false));
   	Path p(r,vector<Pathinfo>(c));
   	int my_id = me(); //My ID;
   	int builder = true;
   	int num_barricades = 0;
   	
  	if (is_day()) {
  	//At day take warriors try to look for weapons (Priority).
  	vector<int> w = warriors(me()); 
  	for (int id1: w) { //Iterate over all the warriors.
             Pos p1 = citizen(id1).pos; //The position on the board
             Pos objective;
  	     
  	     if (search_weapons(p1,mark,objective,p,my_id)) { //Try to look for guns and keep all of them to make them disappear.
  	     	 	Dir direction = calcul_direction(p1,objective,p);
  	     	 	move(id1,direction);
  	     }  	     	    
  	     //Try to look for money(1) or food(2)   	     		
  	     else {
  	       	mark = Vis (r, vector<bool>(c,false));
  	     	if (search_loot(p1,mark,objective,p,my_id)) {          	     		         
  	     	      Dir direction = calcul_direction(p1,objective,p);
  	     	      move(id1,direction);
  	        }
  	     }
  	     	
  	     	/*else { //Move random
  	     	      bool some_dir = false;
	    	      for (uint i = 0; not some_dir and i < dirs.size(); ++i) {
	    	      	   Dir d = dirs[i];
	  		   if (pos_ok(p1+d)and cell(p1+d).id == -1 and cell(p1+d).resistance == -1 and cell(p1+d).type == Street) { 
	  			some_dir = true;
	  			move(id1,d);
		  	   }		  
  	     	      }   
  	       */
  	      //  }
  	        
  	        
  	      }	 	        		          			     			 	     	        
  	// }	
  	
  	// Builders try to look for money, food and build barricades.
  	vector<int> b = builders(me());
  	for (int id2: b) { //Iterate over all the builders.
  	       Pos objective;
  	       Pos p2 = citizen(id2).pos; //The position on the board
  	       if (search_loot(p2,mark,objective,p,my_id)) { 	 
  	       	      Dir direction = calcul_direction(p2,objective,p);
  	     	      move(id2,direction);
  	       }
  	       else { //Build some barricades
  	       	    if (num_barricades < max_num_barricades ()) {
  	       	    	for (Dir d: dirs) {
  	       	    		Pos np = p2 + d;  //Posicion del ciudadano + el despazamiento
	      	    		if (pos_ok(np) and cell(np).id == -1 and cell(np).type == Street) { // pos_ok checks np is in the board, but here something else would be needed to make   
	      	    	        	build(id2,d);    // sure we can construct - > Action.hh
	      	    	        	++num_barricades;
	      	    		}
	      	    	}
  	       	    }
  	       	    else {
  	       	   	if (cell(p2+Up).resistance != -1 and cell(p2+Up).b_owner == my_id) move(id2,Up);
  	       	   	else if (cell(p2+Down).resistance != -1 and cell(p2+Down).b_owner == my_id) move(id2,Down);
  	       	    	else if (cell(p2+Left).resistance != -1 and cell(p2+Left).b_owner == my_id) move(id2,Left);
  	       	    	else if (cell(p2+Right).resistance != -1 and cell(p2+Right).b_owner == my_id) move(id2,Right);  
  	       	    }	       	        	       	    
  	       }  	
  	 }  
      }
      else {
      	vector<int> w = warriors(me()); 
      	for (int id1:w) {
      		bool bazooka = false;
      		bool gun = false;
      	 	Pos p1 = citizen(id1).pos; //The position on the board
             	Pos objective;
      		  		
      		if (citizen(id1).weapon == Bazooka){ //Bazooka
      			bazooka = true;
      			//Try to look for Hammer guys and later gun guys.
      			if (search_enemies(p1,mark,objective,p,my_id,dirs,bazooka,gun)) {
      				 Dir direction = calcul_direction(p1,objective,p);
  	     	     		 move(id1,direction);
  	     	     		 mark = Vis (r, vector<bool>(c,false));
      			}    			
      		}
      		
      		else if (citizen(id1).weapon == Gun) { //Gun
      			gun = true;
      			if (search_enemies(p1,mark,objective,p,my_id,dirs,bazooka,gun)) {
      				 Dir direction = calcul_direction(p1,objective,p);
  	     	     		 move(id1,direction);
  	     	     		 mark = Vis (r, vector<bool>(c,false));
      			}
      			//IN case there aren't hammers hide barricades.
      			//else {
      			//}    
      		}
      		else { //Hammer
  	    	     if (search_enemies(p1,mark,objective,p,my_id,dirs,bazooka,gun)) {
      				 Dir direction = calcul_direction(p1,objective,p);
  	     	     		 move(id1,direction);
  	     	     		 mark = Vis (r, vector<bool>(c,false));
      		     }  	    	       	    	     
  	    	 }	     		      		     		      	
      	}
      	vector<int> b = builders(me());
      	for (int id2: b) {
      		//const Pos& player, Vis& visit, Pos& objective, Path& p, const int my_id, const vector<Dir>& vector
      		Pos p2 = citizen(id2).pos;
      		Pos objective;
      		if (cell(p2).resistance == -1) {
      			if (search_barricades(p2,mark,objective,p,my_id,dirs)) {
      			 	 Dir direction = calcul_direction(p2,objective,p);
  	     	     		 move(id2,direction);
  	     	     		 mark = Vis (r, vector<bool>(c,false));
      			} 
      			else { //Search loot
      			   mark = Vis (r, vector<bool>(c,false));
      			   if (search_loot(p2,mark,objective,p,my_id)) { 	 
  	       	      		Dir direction = calcul_direction(p2,objective,p);
  	     	      		move(id2,direction);
  	     	      		mark = Vis (r, vector<bool>(c,false));
  	      		    }
      		        }    	  
      		}
      	}
      		
      }     	  
      
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
