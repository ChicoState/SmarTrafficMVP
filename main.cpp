#include <iostream>
#include <string>
#include "ObserverPattern.hpp"

// The LightData is a model that represents whether a light is signaling to "go" or not
class LightData: public Observable
{
private:
  bool go;

public:
  
  LightData()
  { 
    go = false;
  }
  
  void setSignal(bool signal)
  {
    this->go = signal;
  }

  bool signalGo()
  {
    return go;
  }
};


//The view displays the directions to the user (drivers), like the traffic light itself
class TrafficLight
{
public:
  void showSignal(std::string direction, bool canGo) //interface override
  {
    std::string command;
    if( canGo )
    {
      command = "GO";
    }
    else
      command = "STOP";
    std::cout<<"The "<<direction<<" traffic should: "<<command<<std::endl;
  }
};



//The controller delegates both north-south and east-west pairs of signals
//and binds the sensor's actions to updating the views
class IntersectionController: public IObserver
{
private:
  LightData northSouth;
  LightData eastWest;
  TrafficLight northbound;
  TrafficLight southbound;
  TrafficLight eastbound;
  TrafficLight westbound;

public:
  IntersectionController()
  {
    northSouth.setSignal(true); //Initially, North and Southbound "go"
    eastWest.setSignal(false);
    northSouth.addObserver(this);
  }

  void goNorthSouth()
  {
    northSouth.setSignal(true);
    eastWest.setSignal(false);
    northSouth.notifyObservers();
    eastWest.notifyObservers();
  }

  void goEastWest()
  {
    eastWest.setSignal(true);
    northSouth.setSignal(false);
    eastWest.notifyObservers();
    northSouth.notifyObservers();
  }

  void update()
  {
    northbound.showSignal("North",northSouth.signalGo());
    southbound.showSignal("South",northSouth.signalGo());
    eastbound.showSignal("East",eastWest.signalGo());
    westbound.showSignal("West",eastWest.signalGo());
  }
};



//Manually controlling the traffic light sensors
int main()
{
  IntersectionController intersection;
  char input;
  bool valid = true;
  do{
    std::cout<<"Who should go? (n,s,e,w)\n";
    std::cin>>input;
    if( input == 'n' || input == 's')
      intersection.goNorthSouth();
    else if( input == 'e' || input == 'w' )
      intersection.goEastWest();
    else
    {
      std::cout<<"Shutting down.\n";
      valid=false;
    }
  }while(valid);
  return 0;
}
