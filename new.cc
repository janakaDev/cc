#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"			
#include "ns3/mobility-module.h"  

//udp      
#include "ns3/constant-position-mobility-model.h"
//udp

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("one");   

int main(int argc,char *argv[])
{
    CommandLine cmd;
    cmd.Parse(argc,argv);

    Time::SetResolution(Time::NS);
     // UDP ECO Start
    /*LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);*/
    //UDP ECO Stop

    // UDP Start
    /*LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);*/   
    //UDP Stop    
   

    NS_LOG_INFO("Creating Nodes.."); //to print some text

    NodeContainer nodes;
    nodes.Create(2);

   

        Vector temp0(1,1,0);
        Ptr<Node> n0=nodes.Get(0);
        Ptr<ConstantPositionMobilityModel> p0= n0->GetObject<ConstantPositionMobilityModel>();
        p0=CreateObject<ConstantPositionMobilityModel>();
        n0->AggregateObject(p0);
        p0->SetPosition(temp0);

        Vector temp1(2,2,0);
        Ptr<Node> n1=nodes.Get(1);
        Ptr<ConstantPositionMobilityModel> p1=n1->GetObject<ConstantPositionMobilityModel>();
        p1=CreateObject<ConstantPositionMobilityModel>();
        n1->AggregateObject(p1);
        p1->SetPosition(temp1);

    


    
    //pointToPoint start
   /* PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay",StringValue("20ms"));
    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);*/
    //pointToPoint stop

    //csma start
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate",StringValue("5Mbps"));
    csma.SetChannelAttribute("Delay",StringValue("20ms"));
    NetDeviceContainer devices;
    devices = csma.Install(nodes);
     //csma stop



    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0","255.255.255.0");

    Ipv4InterfaceContainer interfaces = address.Assign(devices);
    

    // UDP echo server start    
    /*UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));*/
    // UDP echo server end

    // UDP server start
    /*UdpServerHelper server (4000);
    ApplicationContainer apps = server.Install (nodes.Get (1));
    apps.Start (Seconds (1.0));
    apps.Stop (Seconds (10.0));*/
     // UDP server end

    // TCP server start
    PacketSinkHelper sink ("ns3::TcpSocketFactory",Address(InetSocketAddress (Ipv4Address::GetAny (), 10)));
    ApplicationContainer app = sink.Install (nodes.Get(0));
    app.Start (Seconds (1.0));
    app.Stop (Seconds (3.0));
    // TCP server Stop

    // UDP echo client Start
    /*UdpEchoClientHelper echoClient(interfaces.GetAddress(1),9);
    echoClient.SetAttribute("MaxPackets",UintegerValue(1));
    echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize",UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));*/
     // UDP echo client Stop


     // UDP client Start
       /* UdpClientHelper client (interfaces.GetAddress(1),4000);
        client.SetAttribute ("MaxPackets", UintegerValue (320));
        client.SetAttribute ("Interval", TimeValue (Seconds (0.05)));
        client.SetAttribute ("PacketSize", UintegerValue (1024));
        apps = client.Install (nodes.Get (0));
        apps.Start (Seconds (2.0));
        apps.Stop (Seconds (10.0));*/
     // UDP client Stop


     //TCP Client start
    OnOffHelper onOffHelper ("ns3::TcpSocketFactory", Address(InetSocketAddress (Ipv4Address ("10.1.1.0"), 10)));
    onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

    onOffHelper.SetAttribute ("DataRate",StringValue ("2Mbps"));
    onOffHelper.SetAttribute ("PacketSize",UintegerValue(1024));    
    app = onOffHelper.Install (nodes.Get (1));
    
    app.Start (Seconds (1.0));
    app.Stop (Seconds (3.0));
    //TCP Client stop


    //wireshark start
    csma.EnablePcapAll("tcpAnim");
    //wireshark stop


	//Animation

	AnimationInterface anim("neww.xml");
	anim.UpdateNodeSize(0,0.3,0.3);
    anim.UpdateNodeSize(1,0.3,0.3);

    //anim.UpdateNodeColor (0,190,0,210);  

    anim.UpdateNodeDescription(0,"Sender");
    anim.UpdateNodeDescription(1,"Receiver");
	//end Animation

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}


