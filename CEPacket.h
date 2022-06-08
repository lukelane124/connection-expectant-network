/* Connection Expectant Network packet */

/* packet priority indicators */
typedef enum
{
    PRIORITY_UNKNOWN = 0,
    PRIORITY_MINIMAL = 1,                       // It would be cool if you could deliver this packet but I won't lose sleep if you don't even get it.
    PRIORITY_HELLO = 10,                        // First contact packet. A "Are you there?" type ping message.
    PRIORITY_GOODBYE = 20,                      // Last contact message. A "I'm leaving this region..."
    PRIORITY_SLEEP = 50,                        // I'm, or You will be unavailable but don't remove me from your routes unless you don't hear back within milkman time.
    PRIORITY_WAKE = 55,                         // I'm awake and ready for mail.
    
    PRIORITY_FILE_TRANSFER_BLOCK = 100,         // Packet that is a sub-packet of a much larger packet.

    PRIORITY_DELIVERY_OFFLINE_UPDATE = 120,     // I'm going offline remove me from your local reoutes.
    PRIORITY_DELIVERY_ROUTING_UPDATE = 128,     // I have network informaiton to share.
    PRIORITY_DELIVERY_SUCCESS_FAIL = 129,       // Status of high priority packets.

    PRIORITY_MOVE_AWAY = 200,                   // There is some level of danger here. Be aware.
    PRIORITY_MOVE_SAFETY = 210,                 // Iminent percieved/expected danger, hide away to safety.
    PRIORITY_TELEMETRY = 240,                   // I'm here, beware. Intended for use by vessels which could collide or are required to shout their locations.
    PRIORITY_FIRE = 245,                        // I'm / You're experiencing a disater be aware or send help.
    PRIORITY_COLLISION_WARNING = 253,           // I've detected a likelihood of collision. You may never hear me again.
    PRIORITY_MAX = 255                          // If you do one thing, deliver this message to your neighbors. Endemic routing.
} cep_priority_e;

typedef enum
{
    
} ce_zone_area_e;
/*
uint16_t areaSpatialDivision;
UpperNible - 8 bits for slice West to East 
LowerNible - 8 bits for rough South to North _slice_ equal area region indication.

uint8_t zonalElevationDivision
HighestHalfNibble (ElevationZone) - 2 bits Representing a fractional distance of the diameter of the sphere of the zonalDivisionSphere
for Planet 30/1000th of diamater to surface = 0, surface to 1/100th diameter of sphere = 1, up to 1/15 diameter of sphere = 2, Centered at 100x sphere diameter = 4
LowerGreedyNibble (Approximate inter_zonal distance) - 6 bits equdistance divisions from Lowest Point in the elevation zone to highest.



uint8_t zonalDivison
2 bits - planet centric = 0, PlanetElipseCentric = 1, solCentric= 2, galaxyCentric = 3
6 bits +- 3 bits West to East from: Prime Meridian, Ellipse focus closest to sun - toward sun from earth at closest point, Center of galaxy to Canis Major Dwarf Galaxy is 0 East degrees when projected onto the galactic plane North is from Galactic center towards coma Bereneces.
       |
       +- 3 bits for rough South to North _slice_ equal distance (between poles) region indication. (45 degrees resolution)

*/
typedef enum
{
    ZONE_LOCAL = 0,         // Neighborhood.
    ZONE_CITY,              // City or Regional City Area.
    ZONE_PROVINCE,          // Rough geographical country region.
    ZONE_COUNTRY,           // Somewhere in the country.
    ZONE_MOON,              // Somewhere between the surface and the moon
    ZONE_SYSTEM_PLANET,     // Somewhere within 100X the distance from planet's surface to moon from planet's surface
    ZONE_LOCAL_SYSTEM,      // Within the solar system of originating planet. to 5X the systems known diameter+
    ZONE_OUTER_SYSTEM,      // Voyager 2+
} ce_zone_e;

typedef struct 
{
    uint8_t zone;
    uint8_t zonalDivision;
    uint16_t areaSpatialDivision;
    uint8_t zonalElevationDivision;
} ce_zonal_sphere_t;

typedef struct 
{
    uint8_t zone;
    uint8_t zonalAreaDefinition; // 4 bits upper determin northSouth, 4 bits lower determine WestEast;
    uint8_t areaSpatialNorthSouth; // 8 bits determine northSouth
    uint8_t areaSpatialWestEast; // 8 bits determine WestEast;
    uint8_t areaSpatialSphere; // 3 bits for circular division of indicated region. Region 0 is the region just clockwise of dueNorth (NorthNorthEast)
                               // 3 bits for distance away from dead center of area. (area radius/8)
                               // 2 bits for reception radius (area radius/6)/4 is the step size.
} ce_zonal_local_t;



typedef union
{
    uint8_t data[9];
    
} CEZone_u;



typedef struct ce_packet_s
{
    // Populated by the originating Node.
    // These values are used to indicate the "generation timestamp"
    uint8_t _yearsSince2020;
    uint8_t _weekOfYear;
    uint8_t _daysSinceSunday;

    uint32_t minSegmentationSize;   // Useful for FEC with blocks of an expected size, getting blocks smaller than this would not be much help.``

    // Delivery metrics
    uint32_t _maxMilkmanSeconds;            // The number of seconds since packet generation to attempt delivery. U32_MAX is verify delivery based on priority but make every effort to get this packet to the destination.
    cep_priority_e _packetPriority; // U8

    // Collisions are allowed due to geo-spatial expected densities.
    // The Application layer should encode some further packet information indicating some _better_ known id's for the involved nodes.
    uint64_t destinationNodeID;
    uint64_t originatingNodeID;

    ce_zone_t expectedDestinationRegion;          // Region where I expect to find this node when it will be ready to recieve this packet.
    uint32_t expectedDestinationTransitStart;   // Approximate number of seconds since packet generation where I expect the destination node to be in the desination region.
    uint32_t expectedDestinationTransitEnd;     // Approximate number of seconds since packet generation that if passed the priority should be lowered unless traffic is heard from this node.
    ce_zone_t possibleDestinationRegions[4];      // Possible alternate regions where this node might be found during the defined timeframe.



} det_packet_t;