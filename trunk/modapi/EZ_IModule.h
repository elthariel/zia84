#ifndef __IMODULE_H__
# define __IMODULE_H__

#include <string>
#include <map>
#include <EZ_IBuffer.h>

#if defined(_WIN32) || defined(WIN32)
# define EXPORT __declspec(dllexport)
#else
# define EXPORT
#endif

class   EZ_IModule
{
    public:
        enum        ModuleLevel                                                    // modules levels... (or layers if you want...)
        {
            EZ_IN = 1, EZ_PROCEED, EZ_OUT
        };

        virtual                         ~EZ_IModule(){}                            // EZ_IModule cannot be instantiated, thus we have a virtual destructor in order to destroy the object.

        virtual bool                    needProceed() = 0;                         // returns true if the current module needs to proceed the current buffer, else false
        virtual bool                    proceed() = 0;                             // Applies the current module treatement to the current buffer. returns true if everything gone OK

        virtual const EZ_IModule&       getNext() const = 0;                       // returns a reference to the following module
        virtual bool                    setNext(EZ_IModule &) = 0;                 // used to set the next module. returns true if everything gone OK

        virtual	EZ_IBuffer              *popBuffer() = 0;                          // returns the next buffer that need to be processed or NULL if none
        virtual	bool                    pushBuffer(EZ_IBuffer &) = 0;              // pushes the current buffer taken in param in its list of buffer. returns true if everything gone OK

        virtual std::string             getModuleName() const = 0;                 // returns the module's name
        virtual std::string             getModuleDescription() const = 0;          // returns the module's description (ex: "this module writes 'ok' on STDOUT then turns on the coffee machine")
        virtual EZ_IModule::ModuleLevel getModuleLevel() const = 0;                // returns the module's level (or layer...)

        // Set one element of the server configuration. 1st value: key. 2nd value: value. (e.g.: setFullServerConfiguration("RootDirectory", "/var/www")
        virtual void                    setServerConfiguration(const std::string&, const std::string&) = 0; 

        // Re-initialize all local variable to their default value expected the server configuration or some usefully variable.
        virtual void                    clean(void) = 0;
};

extern "C"                                       // .dll/.so 's entry points: (for compliance)
{
    EXPORT EZ_IModule	*getInstance();          // returns an instance of the module...
}

#endif /* __IMODULE_H__ */
