
//#include <rtt/rtt/ComponentLoader.hpp>
#include <rtt/deployment/ComponentLoader.hpp>
#include <rtt/TaskContext.hpp>
#include <rtt/Property.hpp>
#include <rtt/OutputPort.hpp>
#include <rtt/OutputPort.hpp>
//#include <rtt/Method.hpp>
#include <rtt/RTT.hpp>
#include <rtt/scripting/ProgramInterface.hpp>
#include <rtt/scripting/ScriptingService.hpp>
#include <boost/pointer_cast.hpp>

using namespace Orocos;

/**
 * The Controller component from the example in the Deployment
 * Component manual.
 */
class ControllerType
    : public TaskContext
{
    InputPort<double> SensorValues;
    OutputPort<double> SteeringSignals;
    //Property<double> gain;
    double gain;

    // Generates a sine, used in the controller-program.ops script
    double tmp;
    double nextPosition() {
        double w = 1.0 * 2 * 3.14;
        tmp += 0.001;
        //std::cout<<"call nextPosition()"<<std::endl;
        #if 0
       double in_data =0;
        FlowStatus rv = SensorValues.read(in_data);
        if(rv == NewData)
        {
           std::cout<<"new get data is:"<<in_data<<std::endl;
        }
        #endif

        //std::cout<<"SensorValues  in:"
        return 5*sin( w * tmp);
    }
public:
    ControllerType(std::string name)
        : TaskContext(name),
          //SensorValues("SensorValues"),
          //SteeringSignals("SteeringSignals", 30),
          //gain("gain","The proportional gain."),
          tmp(0.0)
    {
        this->addProperty("gain",gain).doc("gain param");
        //this->properties()->addProperty(&gain);

        this->ports()->addPort("SensorValues", SensorValues).doc("1D Measurement of this controller.");
        this->ports()->addPort( "SteeringSignals",SteeringSignals).doc("1D Action of this controller");
        this->addOperation("nextPosition",&ControllerType::nextPosition,this,OwnThread);
       // this->methods()->addMethod( method("nextPosition", &ControllerType::nextPosition,this), "Returns a reference position.");
    }

    // The implementation of this component is completely in the controller-program.ops script
    bool startHook()
    {
        //base::ProgramInterfacePtr pi = this->engine()->programs()->getProgram("ControllerAction");
       
        RTT::scripting::ScriptingService::shared_ptr ss = boost::dynamic_pointer_cast<scripting::ScriptingService>(this->provides()->getService("scripting"));
        RTT::scripting::ProgramInterfacePtr pi = ss->getProgram("ControllerAction");
        
        if (pi)
        {

            bool ret = pi->start();
            if(ret)
            {
                std::cout<<"Program -- ControllerAction start suceess"<<std::endl;
            }
            return ret;
        }

        return false;
    }

    void stopHook()
    {
        //base::ProgramInterfacePtr pi = this->engine()->programs()->getProgram("ControllerAction");
        RTT::scripting::ScriptingService::shared_ptr ss = boost::dynamic_pointer_cast<scripting::ScriptingService>(this->provides()->getService("scripting"));
        RTT::scripting::ProgramInterfacePtr pi = ss->getProgram("ControllerAction");
        //RTT::scripting::ProgramInterfacePtr pi = this->engine()->programs()->getProgram("ControllerAction");
        if (pi)
            pi->stop();
    }
        
};

ORO_CREATE_COMPONENT(ControllerType)
