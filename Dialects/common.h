#include <cstdint>

enum VehicleTypes
{
	QUAD_COPTER,
	ROTARY_WING_1,
	ROTARY_WING_2,
	TILT_ROTOR,
	FIXED_WING_1,
	FIXED_WING_2,
	GROUND_VEHICLE,
	SURFACE_VEHICLE_1,
	SURFACE_VEHICLE_2,
	UNDERWATER_VEHICLE_1,
	UNDERWATER_VEHICLE_2,
};

typedef enum
{

	_2D_4_CONFIG,
	_2D_6_CONFIG,
	_3D_6_CONFIG,
	_3D_8_CONFIG,

} OASConfig;

typedef struct
{
	double Accel[3];
	double Gyro[3];
	double Mag[3];
	double AltitudePressureSensor;
	double DifferentialPressureSensor;
} AttitudeSensorIn_t;

typedef struct
{
	std::uint8_t IMUISR : 1;
	std::uint8_t GPSISR : 1;
	std::uint8_t APSISR : 1;
	std::uint8_t DPSISR : 1;
	std::uint8_t PSISR : 1;
	std::uint8_t CAMISR : 1;
	std::uint8_t ENGISR : 1;
	std::uint8_t ESCISR : 1;

} ISR;

typedef struct
{
	OASConfig PSConfig;
	double *ProximitySensor;
	double ProximityMinimum;
} OAS_t;

typedef struct
{
	double VehicleLatitude;
	double VehicleLongitude;
	double VehicleSpeed;
	double VehicleCOG;
	uint32_t NGeofenceCoordinates;
	double GeofenceCoordinates[][2];
} GPS_t;