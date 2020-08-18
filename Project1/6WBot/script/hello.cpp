#include <gazebo/gazebo.hh>

namespace gazebo
{
  class WorldPlugin6WBot : public WorldPlugin
  {
    public: WorldPlugin6WBot() : WorldPlugin()
            {
              printf("Hello Office!\n");
            }

    public: void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
            {
            }
  };
  GZ_REGISTER_WORLD_PLUGIN(WorldPlugin6WBot)
}
