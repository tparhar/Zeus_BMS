% Set Resistivities

% Resistivity of Copper (Ohm.m)
rho_copper = 1.68e-8;
% Resistivity of Aluminum (Ohm.m)
rho_aluminum = 2.82e-8;

% Length of the busbar (meters)
busbar_length = 0.5;
% Cross-sectional area of busbar (square meters)
busbar_area = 1;

% Resistance of copper (Ohm)
R_copper = rho_copper * (L / A);
% Resistance of aluminum (Ohm)
R_aluminum = rho_aluminum * (L / A);

% Current that the busbar will experience
busbar_current = 100;

% Temperature coefficient of copper (1/K)
alpha_copper = 3.93e-3;
% Temperature coefficient of copper (1/K)
alpha_aluminum = 3.8e-3;

% Weight of busbar (kg)
busbar_weight = 1;

% Copper specific heat capacity (J/(K*kg))
copper_specific_heat_capacity = 385;