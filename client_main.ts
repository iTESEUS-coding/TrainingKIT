import {
    OPCUAClient,
    MessageSecurityMode,
    SecurityPolicy,
    AttributeIds,
    makeBrowsePath,
    ClientSubscription,
    TimestampsToReturn,
    MonitoringParametersOptions,
    ReadValueIdLike,
    WriteValueLike,
    ClientMonitoredItem,
    DataValue
} from "node-opcua";

const connectionStrategy = {
    initialDelay: 1000,
    maxRetry: 1
};

const options = {
    applicationName: "MyClient",
    connectionStrategy: connectionStrategy,
    securityMode: MessageSecurityMode.None,
    securityPolicy: SecurityPolicy.None,
    endpoint_must_exist: false
};

const client = OPCUAClient.create(options);
const endpointUrl = "opc.tcp://172.21.50.66:4840";

async function main() {
    try {
        // step 1 : connect to
        await client.connect(endpointUrl);
        console.log("connected !");


        // step 2 : createSession
        const session = await client.createSession();
        console.log("session created !");


        // step 3 : browse
        const browseResult = await session.browse("RootFolder");

        console.log("references of RootFolder :");
        for (const reference of browseResult.references) {
            console.log("   -> ", reference.browseName.toString());
        }


        // step 4 : read a variable with readVariableValue
        const maxAge = 0;
        const nodeToRead = {
            nodeId: "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.AUTOMAN_M",
            attributeId: AttributeIds.Value
        };

        const dataValue = await session.read(nodeToRead, maxAge);
        console.log(" value ", dataValue.toString());


        // step 4' : read a variable with read
        const dataValue2 = await session.readVariableValue(
            "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.DI13"
        );
        console.log(" value = ", dataValue2.toString());


        // step 5: install a subscription and install a monitored item for 10 seconds
        const subscription = ClientSubscription.create(session, {
            requestedPublishingInterval: 1000,
            requestedLifetimeCount: 100,
            requestedMaxKeepAliveCount: 10,
            maxNotificationsPerPublish: 100,
            publishingEnabled: true,
            priority: 10
        });

        subscription
            .on("started", function() {
                console.log(
                    "subscription started for 2 seconds - subscriptionId=",
                    subscription.subscriptionId
                );
            })
            .on("keepalive", function() {
                console.log("keepalive");
            })
            .on("terminated", function() {
                console.log("terminated");
            });

        // install monitored item

        const itemToMonitor_automan: ReadValueIdLike = {
            nodeId: "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.AUTOMAN_M",
            attributeId: AttributeIds.Value
        };

        const itemToMonitor_automan_W: WriteValueLike = {
            nodeId: "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.AUTOMAN_M",
            attributeId: AttributeIds.Value,
            value:{
                value:{
                    value : true
                }
            }
        };
        //let automan = await session.writeSingleNode("ns=4;s=",itemToMonitor_automan_W)


        const itemToMonitor_DI01: ReadValueIdLike = {
            nodeId: "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.DI01",
            attributeId: AttributeIds.Value
        };

        const itemToMonitor_DI02: ReadValueIdLike = {
            nodeId: "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.DI02",
            attributeId: AttributeIds.Value
        };

        const itemToMonitor_DI03: ReadValueIdLike = {
            nodeId: "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.DI03",
            attributeId: AttributeIds.Value

        };

        const itemToMonitor_DI04: ReadValueIdLike = {
            nodeId: "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.DI04",
            attributeId: AttributeIds.Value
        };

        const itemToMonitor_DI05: ReadValueIdLike = {
            nodeId: "ns=4;s=|var|CODESYS Control for PFC100 SL.Application.GVL.DI05",
            attributeId: AttributeIds.Value
        };

        const parameters: MonitoringParametersOptions = {
            samplingInterval: 100,
            discardOldest: true,
            queueSize: 10
        };

        const monitoredItem_automan = ClientMonitoredItem.create(
            subscription,
            itemToMonitor_automan,
            parameters,
            TimestampsToReturn.Both
        );

        const monitoredItem_automan_W = ClientMonitoredItem.create(
            subscription,
            itemToMonitor_automan_W,
            parameters,
            TimestampsToReturn.Both
        );

        const monitoredItem_DI01 = ClientMonitoredItem.create(
            subscription,
            itemToMonitor_DI01,
            parameters,
            TimestampsToReturn.Both
        );
        const monitoredItem_DI02 = ClientMonitoredItem.create(
            subscription,
            itemToMonitor_DI02,
            parameters,
            TimestampsToReturn.Both
        );
        const monitoredItem_DI03 = ClientMonitoredItem.create(
            subscription,
            itemToMonitor_DI03,
            parameters,
            TimestampsToReturn.Both
        );
        const monitoredItem_DI04 = ClientMonitoredItem.create(
            subscription,
            itemToMonitor_DI04,
            parameters,
            TimestampsToReturn.Both
        );
        const monitoredItem_DI05 = ClientMonitoredItem.create(
            subscription,
            itemToMonitor_DI05,
            parameters,
            TimestampsToReturn.Both
        );

        monitoredItem_automan.on("changed", (dataValue: DataValue) => {
            console.log(" value has changed automan_m: ", dataValue.value.toString());
        });

        monitoredItem_DI01.on("changed", (dataValue: DataValue) => {
            console.log(" value has changed di01: ", dataValue.value.toString());
        });

        monitoredItem_DI02.on("changed", (dataValue: DataValue) => {
            console.log(" value has changed di02: ", dataValue.value.toString());
        });

        monitoredItem_DI03.on("changed", (dataValue: DataValue) => {
            console.log(" value has changed di03: ", dataValue.value.toString());
        });

        monitoredItem_DI04.on("changed", (dataValue: DataValue) => {
            console.log(" value has changed di04: ", dataValue.value.toString());
        });

        monitoredItem_DI05.on("changed", (dataValue: DataValue) => {
            console.log(" value has changed di05: ", dataValue.value.toString());
        });



    } catch(err) {
        console.log("An error has occured : ",err);
    }
}
main();