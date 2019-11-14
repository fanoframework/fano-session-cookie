(*!------------------------------------------------------------
 * [[APP_NAME]] ([[APP_URL]])
 *
 * @link      [[APP_REPOSITORY_URL]]
 * @copyright Copyright (c) [[COPYRIGHT_YEAR]] [[COPYRIGHT_HOLDER]]
 * @license   [[LICENSE_URL]] ([[LICENSE]])
 *------------------------------------------------------------- *)

container.add(
    'config',
    TJsonFileConfigFactory.create(
        getCurrentDir() + '/config/config.json'
    )
);
config := container.get('config') as IAppConfiguration;

 {-----------------------------------------------
   register middleware list for each routes that
   does nothing,
   need to be use factory so each route will have
   different middleware list
   Replace with TMiddlewareCollectionAwareFactory to support middleware
   or replace with your own implementation if required
 ------------------------------------------------}
container.add('appMiddlewares', TMiddlewareListFactory.create());

container.add('encrypter', (TBlowfishEncrypterFactory.create()).secretKey(config.getString('secretKey')));

container.add(
    'sessionManager',
    TCookieSessionManagerFactory.create(
        TJsonSessionFactory.create(),
        container['encrypter'] as IEncrypter,
        container['encrypter'] as IDecrypter,
        config.getString('session.name')
    )
);

container.add(
    GuidToString(IDispatcher),
    TSessionDispatcherFactory.create(
        container['appMiddlewares'] as IMiddlewareLinkList,
        container[GuidToString(IRouteMatcher)] as IRouteMatcher,
        TRequestResponseFactory.create(),
        container['sessionManager'] as ISessionManager,
        (TCookieFactory.create()).domain(config.getString('cookie.domain')),
        config.getInt('cookie.maxAge')
    )
);
