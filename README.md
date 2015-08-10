# Kshell
C++ component framework.   
`kshell` is a component(DLL or SO) loader and container, the loaded component should offer specific export function.

# Component
Component is called Bundle in `kshell`, bundle could be loaded as a loadable object, Windows DLL or Linux SO file, loadable object should offer three export function and a configuration file (named meta.inf).

## Export functions ##
1. newActivator   
   Fork and return a activator.
2. newService   
   Fork and return a service
3. destroyService   
   destroy service

The prototype of export functions：   
	
	BundleFunc void* newActivator();
	BundleFunc void* newService(const char* name);
	BundleFunc void* destroyService(void* service, const char* name);

The name of three export function are configurable in meta.inf, the naming convension is same as C/C++ function naming guide.

## Activator ##

Activator gives bundle a chance to custimize steps of startup and cleanup, developer could write code to define the activator's behavior, below is the activator interface:

	class BundleActivator {
	public:
	    virtual ~BundleActivator() {}
	    virtual void start(BundleContext* context) throw(BundleException) = 0;
	    virtual void stop(BundleContext* context) throw(BundleException) = 0;
	};

## Service ##

Bundle holds multiple services, each service implements specified interface, the interface is the basic unit of service to provide services for other bundle's service.
`newService` fork a new service instance, `destroyService` destroy service instance.

## meta.inf ##

	bundle="caller_bundle"
	version="0.1"
	symbolicName="caller"
	activator="newActivator"
	newService="newService"
	destroyService="destroyService"
	require="mybundle"

1. bundle   
   Loadable object name
2. version   
   Bundle version
3. symbolicName   
   The name referred by other service 
4. activator   
   The export function name of activator
5. newService   
   The export function name of forking new service instance
6. destroyService   
   The export function name of destroying service instance
7. require   
   Bundle dependency（format: bundle-name;bundle-name;...）

# Make bundle

See bundle-sample.   
There are two bundle in example:`my_bundle` and `caller_bundle`，`my_bundle` exports a very simple service `MyService`:

	class MyService {
	public:
		virtual ~MyService() {}
		virtual const std::string& getGreeting() const = 0;
	};
`caller_bundle` fork a `MyService` service in startup phase of activator:

	void BundleActivator_CallerBundle::start(BundleContext* context) throw(BundleException) {
	    _context = context;
	    // fork a 'MyService' and call it
	    AutoService<MyService> myService = loadClass<MyService>(_context, "mybundle", "myservice");
	    if (!myService) {
	        std::cout << "create service 'myservice' failed" << std::endl;
	    }
	    std::cout << "call MyService::getGreeting(): " << myService->getGreeting() << std::endl;
	    // destroy 'MyService'
	    myService.destroy();
	}

	After `kshell` started the console output like below，for now, the `kshell` start as debug mode:
    
	Debugging kshell.
	kshell v.1.0.0
	Root -> [E:\kshell-git\kshell\win-proj\$\..\..\bin\debug]
	Check bundle dependency -> [Resolved]
	Install Bundle -> [mybundle] -> [INSTALLED]
	Install Bundle -> [caller  ]call MyService::getGreeting(): hi caller!
	 -> [INSTALLED]

# The startup of kshell

## Start mode ##

1. Debug mode
2. Daemon mode(Service mode)   
	
kshell -install to install the service(Windows ONLY)   
kshell -start   to start the service   
kshell -stop    to start the service   
kshell -remove  to remove the service(Windows ONLY)   
kshell -debug <params>   to run as a console app for debugging   


`-debug` make `kshell` start as debug mode and print information on screen. On Windows:

	kshell -install

`kshell` will be installed as a Windows service(make sure you have proper operation priviledge)，`kshell` on Linux cannot be installed as a service.

	kshell -remove

Remove `kshell` from service list，`kshell` on Linux cannot be removed.

	kshell -start

Start `kshell` service，On Linux, `kshell` will run as a daemon process，Windows as Windows service.

	kshell -stop

Shutdown `kshell` service.

## Kshell startup ##

When `kshell` starting it will scan the bundle/ directory and parse bundle's `meta.inf`, sort the dependency of bundles via `requrie` attribute, `kshell` will stop when bundle not there or circular dependency exist, if everything is ok `kshell` will load bundle one by one and invoke `start` method of bundle's activator.

## Kshell cleanup ##

`kshell` invoke the `stop` method of bundle's activator by inverted sequence of dependency and unload loadable object(DLL or SO).

# Kshell
C++组件框架.   
`kshell`是组件（DLL或者SO)的加载运行容器，组件需要预先提供特定的导出函数，才能被容器加载。

# 组件
组件在`kshell`内被称为Bundle，每个bundle都作为可加载对象存在，在Windows下为DLL文件，Linux下为SO文件，可加载对象需要提供
3个导出函数及一个配置文件（名为meta.inf).

## 导出函数 ##
1. newActivator   
   建立并返回组件激活器
2. newService   
   建立并返回一个服务
3. destroyService   
   销毁服务

导出函数的原型为：   
	
	BundleFunc void* newActivator();
	BundleFunc void* newService(const char* name);
	BundleFunc void* destroyService(void* service, const char* name);

以上3个导出函数的名称可以配置在meta.inf文件内，函数名称只要符合C/C++函数命名规范即可。

## 激活器 ##

激活器为组件提供了在启动和停止阶段的自定义操作的机会，用户可以在激活器内提供自己组件需要的操作，激活器接口定义为：

	class BundleActivator {
	public:
	    virtual ~BundleActivator() {}
	    virtual void start(BundleContext* context) throw(BundleException) = 0;
	    virtual void stop(BundleContext* context) throw(BundleException) = 0;
	};

## 服务 ##

组件可以建立多个服务，每个服务都是实现了特定的接口，服务以接口为单位为其他组件内的服务提供具体功能。
通过导出函数`newService`产生组件内的某个服务实例，通过`destroyService`销毁某个服务实例。

## meta.inf ##

	bundle="caller_bundle"
	version="0.1"
	symbolicName="caller"
	activator="newActivator"
	newService="newService"
	destroyService="destroyService"
	require="mybundle"

1. bundle   
   组件的可加载对象的名称
2. version   
   组件版本
3. symbolicName   
   组件被其他组件访问时的名字
4. activator   
   激活器导出函数名称
5. newService   
   服务产生函数名称
6. destroyService   
   服务销毁函数名称
7. require   
   依赖的其他组件（格式为组件名;组件名;...）

# 制作组件

参见bundle-sample.   
例子中提供了两个组件:`my_bundle`和`caller_bundle`，`my_bundle`内提供了一个非常简单的服务`MyService`:

	class MyService {
	public:
		virtual ~MyService() {}
		virtual const std::string& getGreeting() const = 0;
	};
`caller_bundle`在激活器启动阶段建立并使用了`MyService`服务:

	void BundleActivator_CallerBundle::start(BundleContext* context) throw(BundleException) {
	    _context = context;
	    // fork a 'MyService' and call it
	    AutoService<MyService> myService = loadClass<MyService>(_context, "mybundle", "myservice");
	    if (!myService) {
	        std::cout << "create service 'myservice' failed" << std::endl;
	    }
	    std::cout << "call MyService::getGreeting(): " << myService->getGreeting() << std::endl;
	    // destroy 'MyService'
	    myService.destroy();
	}

	启动`kshell`后将得到如下输出，当前kshell以调试模式启动:
    
	Debugging kshell.
	kshell v.1.0.0
	Root -> [E:\kshell-git\kshell\win-proj\$\..\..\bin\debug]
	Check bundle dependency -> [Resolved]
	Install Bundle -> [mybundle] -> [INSTALLED]
	Install Bundle -> [caller  ]call MyService::getGreeting(): hi caller!
	 -> [INSTALLED]

# Kshell的启动流程

## Kshell启动方式 ##

1. 调试模式
2. 后台模式   
	
kshell -install to install the service(Windows ONLY)   
kshell -start   to start the service   
kshell -stop    to start the service   
kshell -remove  to remove the service(Windows ONLY)   
kshell -debug <params>   to run as a console app for debugging   


`-debug`将`kshell`以调试模式启动并使用屏幕作为输出. 在Windows系统下，可以通过:

	kshell -install

将kshell安装为Windows服务（确保拥有相应的操作权限），Linux系统下暂时未提供此功能，通过:

	kshell -remove

将`kshell`服务卸载，Linux系统暂时未提供此功能。 可以通过命令：

	kshell -start

启动`kshell`服务，在Linux下`kshell`将以后台进程方式运行，Windows下将以Windows服务运行，可以通过:

	kshell -stop

关闭`kshell`服务.

## Kshell启动过程 ##

`kshell`启动后会自动扫描当前目录下的bundle/目录并解析所有组件的配置文件，通过配置文件内的`require`字段排序组件之间的依赖关系，如果所依赖的组件或者组件有环形的相互依赖关系则停止并报错，如果一切都是正确的，那么`kshell`会按照排序后的顺序逐个加载组件并调用激活器的`start`方法.

## Kshell关闭过程 ##

`kshell`按组件依赖关系的逆序逐个调用激活器的`stop`方法并卸载可加载对象(DLL或SO).
