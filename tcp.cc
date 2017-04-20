#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/drop-tail-queue.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("csma23");   //to print some text we have to import this

int main(int argc,char *argv[])
{
    CommandLine cmd;
    cmd.Parse(argc,argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);

    NS_LOG_INFO("Creating Nodes.."); //to print some text

    NodeContainer nodes;
    nodes.Create(2);


    Vector temp0(1,1,0);
    Ptr<Node> n0 =nodes.Get(0);
    Ptr<ConstantPositionMobilityModel> p0 = n0->GetObject<ConstantPositionMobilityModel>();
    p0 =CreateObject<ConstantPositionMobilityModel>();
    n0->AggregateObject(p0);
    p0->SetPosition(temp0);


    Vector temp1(2,2,0);
    Ptr<Node> n1 =nodes.Get(1);
    Ptr<ConstantPositionMobilityModel> p1 = n1->GetObject<ConstantPositionMobilityModel>();
    p1 =CreateObject<ConstantPositionMobilityModel>();
    n1->AggregateObject(p1);
    p1->SetPosition(temp1);



    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate",StringValue("5Mbps"));
    csma.SetChannelAttribute("Delay",StringValue("2ms"));

    

    NetDeviceContainer devices;
    devices = csma.Install(nodes);

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0","255.255.255.0");

    Ipv4InterfaceContainer interfaces = address.Assign(devices);




    PacketSinkHelper sink("ns3::TcpSocketFactory",Address(InetSocketAddress(Ipv4Address::GetAny(),10)));
    ApplicationContainer app = sink.Install(nodes.Get(0)); 

    
    app.Start(Seconds(1.0));
    app.Stop(Seconds(3.0));




    OnOffHelper onoffhelper ("ns3::TcpSocketFactory",Address(InetSocketAddress(Ipv4Address("10.1.1.1"),10)));

    onoffhelper.SetAttribute ("OnTime",StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoffhelper.SetAttribute ("OffTime",StringValue("ns3::ConstantRandomVariable[Constant=0]"));


    onoffhelper.SetAttribute("DataRate",StringValue("2Mbps"));
    onoffhelper.SetAttribute("PacketSize",UintegerValue(1024));
    app=onoffhelper.Install(nodes.Get(1));





    app.Start(Seconds(1.0));
    app.Stop(Seconds(3.0));


   

	/*AnimationInterface anim("animCSMA.xml");
	anim.SetConstantPosition(nodes.Get(0),2.0,2.0);
	anim.SetConstantPosition(nodes.Get(1),2.0,4.0);
	anim.SetConstantPosition(nodes.Get(2),6.0,4.0);
	anim.SetConstantPosition(nodes.Get(3),6.0,2.0);*/
	//end Animation

    AnimationInterface anim("animation2.xml");
    anim.UpdateNodeSize(0,0.1,0.1);
    anim.UpdateNodeSize(1,0.1,0.1);
  //  anim.UpdateNodeSize(2,0.1,0.1);
   // anim.UpdateNodeSize(3,0.1,0.1);
    anim.UpdateNodeColor(0,255,127,122);
    anim.UpdateNodeColor(1,122,127,122);
   // anim.UpdateNodeColor(2,167,127,122);
   // anim.UpdateNodeColor(3,111,127,122);

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}


//to compile=> ./waf
//to run=> ./waf --run scratch/csma
