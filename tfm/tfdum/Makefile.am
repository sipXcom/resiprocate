lib_LTLIBRARIES = libtfmdum.la
 

libtfmdum_la_LDFLAGS = @LIBTOOL_VERSION_RELEASE@ -export-dynamic
libtfmdum_la_LIBADD = ../repro/libtfmrepro.la
libtfmdum_la_LIBADD += ../../resip/dum/libdum.la
libtfmdum_la_LIBADD += ../../resip/stack/libresip.la
libtfmdum_la_LIBADD += ../../rutil/librutil.la
libtfmdum_la_LIBADD += @LIBSSL_LIBADD@ @LIBPTHREAD_LIBADD@ -ldb_cxx -lcppunit -lNetxx
 

libtfmdum_la_LIBADD += ../repro/libtfmrepro.la
libtfmdum_la_LIBADD += ../libtfm.la
libtfmdum_la_LIBADD += -lpopt
 

libtfmdum_la_SOURCES = $(SHARED_SRC)
 
SHARED_SRC = DumUserAgent.cxx \
        TestDialogEvent.cxx \
        DialogEventExpect.cxx \
        DumUaAction.cxx \
        DumEvent.cxx \
        DumExpect.cxx \
        DumFixture.cxx \
        TestClientSubscription.cxx \
        TestServerSubscription.cxx \
        TestClientRegistration.cxx \
        TestUsage.cxx \
        TestInviteSession.cxx \
        TestClientPublication.cxx \
        TestServerOutOfDialogReq.cxx \
        TestClientPagerMessage.cxx \
        TestServerPagerMessage.cxx \
        TfdTcpTransport.cxx \
        TfdTestSipEndPoint.cxx

libtfmdumincludedir = $(includedir)/tfm/tfdum
nobase_libtfmduminclude_HEADERS = BindHandle.hxx \
        ClientOutOfDialogReqEvent.hxx \
        ClientPagerMessageEvent.hxx \
        ClientPublicationEvent.hxx \
        ClientRegistrationEvent.hxx \
        ClientSubscriptionEvent.hxx \
        DialogEventExpect.hxx \
        DialogEventHandlerEvent.hxx \
        DumEvent.hxx \
        DumExpect.hxx \
        DumFixture.hxx \
        DumUaAction.hxx \
        DumUserAgent.hxx \
        InviteSessionEvent.hxx \
        ServerOutOfDialogReqEvent.hxx \
        ServerPagerMessageEvent.hxx \
        ServerSubscriptionEvent.hxx \
        TestClientPagerMessage.hxx \
        TestClientPublication.hxx \
        TestClientRegistration.hxx \
        TestClientSubscription.hxx \
        TestDialogEvent.hxx \
        TestInviteSession.hxx \
        TestServerOutOfDialogReq.hxx \
        TestServerPagerMessage.hxx \
        TestServerSubscription.hxx \
        TestUsage.hxx \
        TestUserAgent.hxx \
        TfdTcpTransport.hxx \
        TfdTestSipEndPoint.hxx
