# Meetup-bot
Cool dudah for automating our **Google-meet** setup.
It's designed to help set up video calls in medium-size **meeting rooms** by running on a PC connected to your projector/display and your existing webcam (we got inspired by our logitech meetup cam), eliminating the need  to connect your personal PC, promoting Privacy and Ease of Use.
It integrates into your existing **Slack** workspace and can be triggered by tagging `@meetup` in **every public channel**, followed by the action you want to perform: `start call`, `stop call`, `status` or `help`.
* When asked to **start** a call, the bot will do so and reply on the **same channel** (e.g. `#general`) with the link to **join the meet**, ready for all the other team members.
* a call can be started by **every** slack user, except for other **bots** and the same is true for stopping it.

When a message is posted with` @meetup` it triggers a **zap** in zapier.com that then sends a POST request to thinger.io forwarding the message text, its sender name and the channel it was posted in; this data is then redirected to the device on which the software is running where it will trigger the different actions. When a response is ready, it will be sent as a JSON to a webhook back on zapier.com where another zap will take its content and post it on Slack on the specified channel.

##System requirements
* **Random mini PC**, enough power to handle the video stream from the webcam, a Raspberry Pi 3 might even be enough
* **Ubuntu 19**, other versions may work but have not been tested yet
* **Google chrome**:
    * download from https://www.google.com/chrome/
    #### or 
    * download with `wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb`
    * install with `sudo dpkg -i google-chrome-stable_current_amd64.deb`

##Tools/Dependencies
see **Run It** for an easy way to install all of them at the same time
* **cmake**
    * install with `sudo apt  install cmake`
* **git** https://git-scm.com/
    * install with `sudo apt-get install git`
* **xdotool:** https://github.com/jordansissel/xdotool
    * install with `sudo apt-get install xdotool`
* **g++** / **gcc** compiler:  https://www.gnu.org/software/gcc/
   * install with `sudo apt-get install build-essential`
* **libCURL**  https://curl.haxx.se/
   * install with `apt-get install libcurl4-openssl-dev`
* **cJSON**  https://github.com/DaveGamble/cJSON
   * download with `git clone https://github.com/DaveGamble/cJSON` 
   * open the downloaded folder with `cd cJSON`
   * create a build directory `mkdir build`
   * open the new directory `cd build`
   * set building options `cmake .. -DENABLE_CJSON_UTILS=On -DENABLE_CJSON_TEST=Off -DCMAKE_INSTALL_PREFIX=/usr`
   * compie `make`
   * instal `sudo make DESTDIR=$pkgdir install`

## External services
* **G-Suite**: 
    * Create a new account
    * Log into Google Chrome on your device
    * Make sure meet.google.com can access your webcam and microphone
* **Thinger**: https://thinger.io/
    * Create a new account or log into your existing account, note your **user-name** 
    * Go to `devices` > `Add Device`
    * Choose a `Device ID` and note it
    * Input a random description
    * click to `Generate Random Credential` and note it
    * Save by clicking `Add device`
* **Zapier:**  https://zapier.com
    * Create a new account or log-in your existing account
    * create a zap that will forward messages from slack to our bot via **thinger**
        * click on `Make a new zap`
        * Search for `Slack` and select it as your trigger app
        * Select `New Mention` as your trigger action
        * Connect a slack account from your Workspace
        * Edit options selecting `@meetup` for your **User Name**
        * Test/Skip then Add an action step using the `webhooks by zapier` app
        * Create a `POST` request
        * Edit with
            * **URL** : `https://api.thinger.io/v2/users/YOUR_THINGER_USER_NAME/devices/meetup/new-message`
            * **Payload Type**: `JSON`
            * Data: **userName** = `User Real Name`, **text** = `Text`, **channelName** = `Channel Name`, **channelID** = `Channel ID`
            * **Headers**: **Content-Type** = `application/json`, **Authorization** = `Your device credential`
        * launch the script (See **Run It**) and test, it should do things without being able to send a response
        
    * create a zap that will recive HTTP requests and convert them into Slack messages
        * click on `Make a new zap`
        * Search for `Webhooks by zapier` and select it as your **trigger** app and elect `Catch Hook` as the trigger
        * Choose `Slack` as the action app, then `send channel message`
        * test by sending a new message like `ciao @meetup` while the script is running, then add an action step
        * you should already be logged in your slack workspace
        * Edit your template with: 
            * **Channel** : `use a custom value (advanced)`
            * **Custom channel ID**: `channelID` from Step 1
            * **Message Text**: `Text` from Step 1
            * **Bot Name**: `User Name` from Step 1
        * Test & save


## Run it
* open a new terminal window by pressing `Ctrl+Alt+T`
* download the project by typing `git clone https://github.com/mondora-labs/meetup-bot`
* open the project folder `cd meetup-bot`
* configure your account settings:
    * open the configuration file `nano settings.txt`, remember to leave the space between `=` and your data
    * replace `YOUR_USER_ID` with your thinger user name
    * replace `YOUR_DEVICE_ID` with the ID of the device you created
    * replace `YOUR_DEVICE_CREDENTIAL` with the credentials of the device you created
    * replace `YOUR_ZAPIER_HOOK` with the URL you got from zapier.
    * close the editor with `Ctrl + X` and press `Y` then `Enter` to save changes
* add permissions to the scripts with `chmod a=rwx *.sh`
* install all the required tools/libraries with `./install.sh`
* Build image with `g++ -o meetup ./main.cpp -lcurl -lcjson`
* Run with `./meetup`
* Test & enjoy

## Usage & commands
 Insert `@Meetup` in whatever message on Slack to have it forwarded to the meetup bot and analyzed, your message may contain other key words (`begin`, `stop`, `status` ...), even in sparse order, to tell the bot what to do, otherwise it will just answer as if it was asked for `help`
 * `begin`, `start`, `inizia` can be used to start a new call. If one is already running, you will be asked if you want to stop it before being able to start a new one.
 * `stop`, `kill`, `termina`, `chiudi` can be used by every user to stop a call, even if it was started by someone else. If there is no call running, no changes will be made.
 * `status`, `stato` will respond by telling you whether there is a call already running and, in case there is one, will let you know the link to join and who started it
 * `help` will hopefully help you by stating the available commands.
