# Procedural-Generation-of-Cities
This project presents an interface through which users are able to generate cities using procedural generation methods.

The city map is created through a mix of 3 different procedural techniques:

-Terrain: the terrain uses a mixture of Improved Perlin Noise along with a blending function that smoothes out the sudden increases in height
<img width="500" alt="Terrain" src="https://user-images.githubusercontent.com/18645132/233167592-6bae8dc7-2ed0-4e87-aa51-60190d0ac30a.PNG">

-Streets: in order to create the street network, I have made use of L-Systems in order to offer the user the ability to gradually grow the network to the desired size
<img width="1426" alt="Streets" src="https://user-images.githubusercontent.com/18645132/233167970-58ba54ed-ef63-4819-a217-9b616f6bd207.png">
-Buildings: to ensure a high degree of complexity and diversity for the buildings, I have integrated Shape Grammars which make use of substractive geometry
<img width="1420" alt="Buildings" src="https://user-images.githubusercontent.com/18645132/233168818-3d41f690-a005-4bf5-9b2f-77dee61f7f20.PNG">

The main goal of this application is to offer the ability to create a map containing a city and for the user to be able to edit all the generation aspects to their liking.

Currently the project is still in development!

