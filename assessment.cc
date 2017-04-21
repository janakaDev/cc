#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/drop-tail-queue.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("csma2");

int main(int argc,char *argv[])
{

    CommandLine cmd;
    cmd.Parse(argc,argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);

   

    NodeContainer nodes;
    nodes.Create(5);
     NS_LOG_INFO("Creating nodes  ..");


    Vector temp0(1,1,0);
    Ptr<Node> n0=nodes.Get(0);
    Ptr<ConstantPositionMobilityModel> p0=n0->GetObject<ConstantPositionMobilityModel>();
    p0=CreateObject<ConstantPositionMobilityModel>();
    n0->AggregateObject(p0);
    p0->SetPosition(temp0);

    

    Vector temp1(2,2,0);
    Ptr<Node> n1=nodes.Get(1);
    Ptr<ConstantPositionMobilityModel> p1=n1->GetObject<ConstantPositionMobilityModel>();
    p1=CreateObject<ConstantPositionMobilityModel>();
    n1->AggregateObject(p1);
    p1->SetPosition(temp1);

    Vector temp2(3,1,0);
    Ptr<Node> n2=nodes.Get(2);
    Ptr<ConstantPositionMobilityModel> p2=n2->GetObject<ConstantPositionMobilityModel>();
    p2=CreateObject<ConstantPositionMobilityModel>();
    n2->AggregateObject(p2);
    p2->SetPosition(temp2);

    Vector temp3(1,3,0);
    Ptr<Node> n3=nodes.Get(3);
    Ptr<ConstantPositionMobilityModel> p3=n3->GetObject<ConstantPositionMobilityModel>();
    p3=CreateObject<ConstantPositionMobilityModel>();
    n3->AggregateObject(p3);
    p3->SetPosition(temp3);

    Vector temp4(3,3,0);
    Ptr<Node> n4=nodes.Get(4);
    Ptr<ConstantPositionMobilityModel> p4=n4->GetObject<ConstantPositionMobilityModel>();
    p4=CreateObject<ConstantPositionMobilityModel>();
    n4->AggregateObject(p4);
    p4->SetPosition(temp4);


    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate",StringValue("5Mbps"));
    csma.SetChannelAttribute("Delay",StringValue("2ms"));

    NetDeviceContainer devices;
    devices=csma.Install(nodes);

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0","255.255.255.0");

    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    //serv

    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps =echoServer.Install(nodes.Get(0));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    //client

    UdpEchoClientHelper echoClient(interfaces.GetAddress(0),9);
    echoClient.SetAttribute("MaxPackets",UintegerValue(1));
    echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize",UintegerValue(1024));



    ApplicationContainer clientApp1 =echoClient.Install(nodes.Get(1));
    clientApp1.Start(Seconds(3.0));
    clientApp1.Stop(Seconds(10.0));


    ApplicationContainer clientApp2 =echoClient.Install(nodes.Get(2));
    clientApp2.Start(Seconds(5.0));
    clientApp2.Stop(Seconds(10.0));

    ApplicationContainer clientApp3 =echoClient.Install(nodes.Get(3));
    clientApp3.Start(Seconds(5.0));
    clientApp3.Stop(Seconds(10.0));

    ApplicationContainer clientApp4 =echoClient.Install(nodes.Get(4));
    clientApp4.Start(Seconds(3.0));
    clientApp4.Stop(Seconds(10.0));



    AnimationInterface anim2("practice.xml");
    anim2.UpdateNodeSize(0,1.0,1.0);
    anim2.UpdateNodeSize(1,1.0,1.0);
    anim2.UpdateNodeSize(2,1.0,1.0);
    anim2.UpdateNodeSize(3,1.0,1.0);
    anim2.UpdateNodeSize(4,1.0,1.0);

	
	
	
	
	//csma.EnablePcapAll("tcpAnim");

	
	/*
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
		
		
	
	*/
	
	
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

































