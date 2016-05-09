#XML configuration

## Basic structure

The configuration file may contain a lot of elements and some components may define their own elements. The basic structure of the file is following:

```xml
<app_config>
  <settings>
    <default_settings/>
    <global_settings/>
    <project_settings/>
  </settings>

  <scenes>
  </scenes>
</app_config>
```

### Settings

There are three general settings in CogEngine:
* default_settings contains default values
* global_settings contains values for initialization of global components (e.g. logger)
* project_settings may be used by any project but engine doesn't use it

Structure of the settings is following:


```xml
<default_settings>
  <setting name="mySetting">
   <item key="myKey" value="myValue" />
  </setting>
</default_settings>
```

Multiples values can be specified as follows:


```xml
<item key="multiple_vals">
  <value>value1</value1>
  <value>value2</value>
</item>
```

or using the | separator:

```xml
<item key="multiple_vals" value="val1|val2" />
```
or using xml attributes:


```xml
<item key1="value1" key2="value2" />
```

### Scenes

Scenes contain a tree structure of nodes. Each scene must have a unique name and the name of the first scene must be specified in the root element:

```xml
<scenes initial="scene1">
  <scene name="scene1">
    <node img="background.png" />
  </scene>
</scenes>
```

### Nodes

Each node may contain following inner elements: behaviors, children nodes, mesh, transformation and states.

Behaviors are declared by the type of class they represents:


```xml
<node name="myNode">
  <behavior type="MultiAnim" />
</node>
```

Mesh represents a renderable object


```xml
<node name="myNode">
  <mesh type="image" img="figure.png" />
</node>
```


[Home](Readme.md)

