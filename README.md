RestServer for Arduino
----------------------

This lib implements a REST API Server for Arduino + Ethernet in the way that web frameworks implements: "router-oriented"

All the endpoints of the API are implemented by the developer in functions and these are added as routes at the REST Server. This gives you more control of what is happening and gives a smaller compiled output. So, you will not have any "out-of-box" endpoint like http://arduino/digital/1, but you can easily implement, if you want.

Setup
-----

- Connect the Ethernet Shield/Card at your Arduino
- Add the library at your Arduino IDE (Sketch > Import Library > Add...)
- Include these files:
  - RestServer.h
  - UIPEthernet.h OR Ethernet.h
  - Log.h
- Setup the EthernetServer
- Add ```RestServer rest(server);``` to initialize the RestServer. Don't forget to pass the EthernetServer object as argument
- Create functions which your routes will callback to process the data. You need to add the (char * params) as the argument of the function and the return type as void.
  - Example: ```void digital(char * params = "") {;} ```
- At setup():
  - Setup and start your EthernetServer
  - Add the routes of your RestServer. For that use ```rest.addRoute(ANY, "/digital", digital);```
    - The first argument is which HTTP Method you want to use. In this case the route will accept any and will process it in the same way
    - The second argument is the route name. YOU ALWAYS NEED TO PUT THE "/" AT THE BEGINNING (I didn't write any code to "treat" this).
    - The last argument is the function which will be used as callback for the route
- At loop();
  - Just add ```rest.run();```

You can add, by default, 5 routes. If you want to change that, define ROUTES_TOTAL as other number. If you want to see other settings, mainly about the size of the buffers or length of some strings, check the RestSettings.h.

To-do
-----

- Some kind of param treatment: today when you request /digital?t=1&x=2, you will receive at your callback function the string "t=1&x=2", without any treatment
- Optimize the code: today, with the logs enabled, it weights ~21kb (with all necessary libs included)
- Try to do some heavy tests: I did it for a college project, so it is not heavy tested yet
- Route name treatment to check if there is a "/" or not
- Wi-Fi support
- Comment the code

If you want to colaborate, feel free to help me and send pull requests. If you have any doubt, question or suggestion, my e-mail is contato@brunoluiz.net