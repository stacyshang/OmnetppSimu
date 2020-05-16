//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 


//#include "App.h"
#include "String.h"
#include <omnetpp.h>
#include <stdio.h>
#include "..\Messages\Packet_m.h"
using namespace omnetpp;

class App:public cSimpleModule{
  private:
    // configuration
    int myAddress;

    int n; //number of all nodes
    cPar *sendIATime;// time between generating packets
    cPar *packetLengthBytes;// length of one message (fixed! no "volatile" modifier)
    cPar *pattern;
    // state
    cMessage *generatePacket;
    int pkCounter;

    // signals
 //   simsignal_t hopCountSignal;
//    simsignal_t endToEndDelaySignal;
//    simsignal_t sourceAddressSignal;

//  public:


  protected:
    virtual void initialize() ;
    virtual void handleMessage(cMessage *msg) ;
    virtual void finish();

    cStdDev hopStats;
    cStdDev eedStats;
    cStdDev throughput;
};
Define_Module(App);


void App::initialize(){
    //hopCountSignal = registerSignal("hopCounter");
    hopStats.setName("hops");
    eedStats.setName("End-to-End Delay");
    throughput.setName("throughput");

    myAddress = ((int)getParentModule()->getParentModule()->par("rackid"))*10+(int)getParentModule()->par("id");//Node's address
    n =(int)getParentModule()->getParentModule()->getParentModule()-> par ("NumNodes");//network nodes number

    pattern = &par("pattern");
        packetLengthBytes = &par("packetLength");
        sendIATime = &par("sendIaTime");  // volatile parameter
        pkCounter = 0;

        WATCH(pkCounter);
        WATCH(myAddress);

        generatePacket = new cMessage("nextPacket");
        scheduleAt(sendIATime->doubleValue(), generatePacket);//0.0

 //       endToEndDelaySignal = registerSignal("endToEndDelay");
//        hopCountSignal = registerSignal("hopCount");
//        sourceAddressSignal = registerSignal("sourceAddress");

}
void App::handleMessage(cMessage *msg){
    if (msg == generatePacket) {
            // Sending packet

        int destAddress;
        //Pattern1: uniform
        if(pattern->doubleValue() == (double)1){
            destAddress = intuniform(0, n-1);
            while(destAddress == myAddress){
                destAddress = intuniform(0,n-1);
            }
        }
        //Pattren2: neighbor-1
        else if(pattern->doubleValue() == (double)2){
            int prob = intuniform(0, 1);
            if(prob == 0){
                destAddress = (myAddress-1+n) % n;
            }
            else{
                destAddress = (myAddress+1) % n;
            }
        }
        //Pattren3: neighbor2
        else if(pattern->doubleValue() == (double)3){
            int probb = intuniform(0, 9);
            if(probb == 0 || probb == 1 || probb == 2 ||probb == 3){
                destAddress = (myAddress-1+n) % n;
            }
            else if(probb == 4){
                destAddress = (myAddress-2+n) % n;
            }
            else if(probb == 5 || probb == 6 || probb == 7 ||probb == 8){
                destAddress = (myAddress+1) % n;
            }
            else if(probb == 9){
                destAddress = (myAddress+2) % n;
            }
        }
        //Pattren4: adversarial
        else if(pattern->doubleValue() == (double)4){
            int p=intuniform(0,3);
            if (myAddress/10 == 15 || myAddress/10 == 0 || myAddress/10 ==1 || myAddress/10 ==2){
                if (p==0) {destAddress=30+intuniform(0,9);}
                else if (p==1) {destAddress=40+intuniform(0,9);}
                else if (p==2) {destAddress=50+intuniform(0,9);}
                else {destAddress=60+intuniform(0,9);}
            }
            else if (myAddress/10 == 3 || myAddress/10 == 4 || myAddress/10 ==5 || myAddress/10 ==6){
                if (p==0) {destAddress=70+intuniform(0,9);}
                else if (p==1) {destAddress=80+intuniform(0,9);}
                else if (p==2) {destAddress=90+intuniform(0,9);}
                else {destAddress=100+intuniform(0,9);}
            }
            else if (myAddress/10 == 7 || myAddress/10 == 8 || myAddress/10 ==9 || myAddress/10 ==10){
                if (p==0) {destAddress=110+intuniform(0,9);}
                else if (p==1) {destAddress=120+intuniform(0,9);}
                else if (p==2) {destAddress=130+intuniform(0,9);}
                else {destAddress=140+intuniform(0,9);}
            }
            else if (myAddress/10 == 11 || myAddress/10 == 12 || myAddress/10 ==13 || myAddress/10 ==14){
                if (p==0) {destAddress=150+intuniform(0,9);}
                else if (p==1) {destAddress=intuniform(0,9);}
                else if (p==2) {destAddress=10+intuniform(0,9);}
                else {destAddress=20+intuniform(0,9);}
            }
        }
        //Pattren5: 2D neighbor exchange
        else if(pattern->doubleValue() == (double)5){
            int proba = intuniform(0,1);
            if (proba == 0){ //X dimension
                int pp = intuniform(0,1);
                if (pp==0) {
                    if (myAddress/10 == 2 || myAddress/10 == 6 || myAddress/10 == 10 || myAddress/10 == 14){
                        destAddress=((myAddress/10-3+16)%16)*10+intuniform(0,9);
                    }
                    else{
                        destAddress=((myAddress/10+1)%16)*10+intuniform(0,9);
                    }
                }
                else {
                    if (myAddress/10 == 15 || myAddress/10 == 11 || myAddress/10 == 7 || myAddress/10 == 3){
                        destAddress=((myAddress/10+3)%16)*10+intuniform(0,9);
                    }
                    else{
                        destAddress=((myAddress/10-1+16)%16)*10+intuniform(0,9);
                    }
                }
            }
            else if (proba == 1){ //Y dimension
                int pp = intuniform(0,1);
                if (pp==0) {destAddress=((myAddress/10+4)%16)*10+intuniform(0,9);}
                else {destAddress=((myAddress/10-4+16)%16)*10+intuniform(0,9);}
            }
        }
        //Pattren6: all-to-all in Y dimension
        else if(pattern->doubleValue() == (double)6){
            if (myAddress/10 == 15 || myAddress/10 == 0 || myAddress/10 ==1 || myAddress/10 ==2){
                destAddress = intuniform(3,14)*10+intuniform(0,9);
            }
            else if (myAddress/10 == 3 || myAddress/10 == 4 || myAddress/10 ==5 || myAddress/10 ==6){
                destAddress = (intuniform(7,18)%16)*10+intuniform(0,9);
            }
            else if (myAddress/10 == 7 || myAddress/10 == 8 || myAddress/10 ==9 || myAddress/10 ==10){
                destAddress = (intuniform(11,22)%16)*10+intuniform(0,9);
            }
            else{
                destAddress = (intuniform(15,26)%16)*10+intuniform(0,9);
            }
        }
        //Pattren7: all-to-all in X dimension
                else if(pattern->doubleValue() == (double)7){
                    if (myAddress/10 == 15 || myAddress/10 == 0 || myAddress/10 ==1 || myAddress/10 ==2){
                        destAddress = (intuniform(15,18)%16)*10+intuniform(0,9);
                    }
                    else if (myAddress/10 == 3 || myAddress/10 == 4 || myAddress/10 ==5 || myAddress/10 ==6){
                        destAddress = (intuniform(3,6)%16)*10+intuniform(0,9);
                    }
                    else if (myAddress/10 == 7 || myAddress/10 == 8 || myAddress/10 ==9 || myAddress/10 ==10){
                        destAddress = (intuniform(7,10)%16)*10+intuniform(0,9);
                    }
                    else{
                        destAddress = intuniform(11,14)*10+intuniform(0,9);
                    }
                }

        else{
            EV<<"Error! pattern can not be recognized! "<<endl;
        }
        //......
            char pkname[40];
            sprintf(pkname, "myAddress-%d-to-%d", myAddress, destAddress);

            EV << "generating packet " << pkname << endl;

            Packet *pk = new Packet(pkname);
            pk->setByteLength(packetLengthBytes->longValue());
            pk->setSrcAddr(myAddress);
            pk->setDestAddr(destAddress);
            //pk->setHopCount(pkCounter);//�����һ��
            send(pk, "out");

            scheduleAt(simTime() + sendIATime->doubleValue(), generatePacket);

    }
    else {
            // Handle incoming packet
        if(dynamic_cast <Packet *> (msg) != NULL){
            Packet *pk = check_and_cast<Packet *>(msg);

            pkCounter++;//ͳ���հ�
            throughput.collect(pkCounter);

            int hopCount = pk->getHopCount()+1;//ͳ������
            EV << "App Address: "<< myAddress << " received packet " << pk->getName() << " after " << hopCount << " hops" << endl;
            hopStats.collect(hopCount);

            simtime_t eed = simTime() - pk->getCreationTime();//ͳ��ʱ��
            eedStats.collect(eed);
            delete pk;
//            emit(endToEndDelaySignal, simTime() - pk->getCreationTime());
//            emit(hopCountSignal, pk->getHopCount());
//            emit(sourceAddressSignal, pk->getSrcAddr());
        }
    }

}
void App::finish(){
    throughput.record();
      eedStats.record();
      hopStats.record();
}

